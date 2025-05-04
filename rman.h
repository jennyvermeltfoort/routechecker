/**
 * @file rman.h
 * @author Jenny Vermeltfoort (jennyvermeltfoort@outlook.com)
 * https://github.com/jennyvermeltfoort
 * @brief Route manager.
 * @version 0.1
 * @date 2025-05-04
 * @copyright Copyright (c) 2025
 */

#ifndef __RMAN_H
#define __RMAN_H

#include "stdbool.h"

#define AANTAL_STATIONS_MIN 1    // Minimaal toegestane stations.
#define AANTAL_STATIONS_MAX 500  // Maximaal toegestane stations.

/**
 * @brief Handle waarmee er wordt gepraat met de implementatie. Gebruik rman_maak om deze handle te
 * alloceren.
 */
typedef struct {
    int oj[AANTAL_STATIONS_MAX];   // De goedkoopste strategie, hoeveelheid te tanken bij station i.
    bool ok[AANTAL_STATIONS_MAX];  // De goedkoopste strategie, of een joker (true: ja, false: nee)
                                   // te gebruiken bij station i.
    int as;                        // aantal stations
} rman_handle_t;

/**
 * @brief Alloceert geheugen en maakt een geinitializeerde route manager aan, leest de puzzel uit
 * een bestand.
 * @param pad Het pad naar het bestand waar de puzzel in staat omschreven, moet voldoen aan de
 * structuur beschreven in het verslag.
 * @return const rman_handle_t* Een handle naar een geinitializeerde route manager, NULL wanneer de
 * puzzel geen oplossing heeft of wanneer het bestand niet ingelezen kan worden of niet voldoet aan
 * de structuur.
 */
const rman_handle_t* rman_maak(const char* pad);

/**
 * @brief Vernietig en dealloceer het geheugen van de route manager hangend aan de handle.
 * @param h Een handle naar een geinitializeerde route manager.
 */
void rman_verwoest(const rman_handle_t* const handle);

/**
 * @brief Print de informatie van een puzzel.
 * @param h Een handle naar een geinitializeerde route manager.
 */
void rman_print(const rman_handle_t* h);

/**
 * @brief Los de puzzel op met een top down algoritme.
 * @param h Een handle naar een geinitializeerde route manager.
 * @param jokers De maximaal hoeveelheid jokers wat gebruikt mag worden.
 * @return int De kosten van de goedkoopste route.
 */
int rman_los_op_td(const rman_handle_t* h, int jokers);

/**
 * @brief Los de puzzel op met een bottom up algoritme.
 * @param h Een handle naar een geinitializeerde route manager.
 * @param jokers De maximaal hoeveelheid jokers wat gebruikt mag worden.
 * @return int De kosten van de goedkoopste route.
 */
int rman_los_op_bu(const rman_handle_t* h, int jokers);

#endif  // __RMAN_H