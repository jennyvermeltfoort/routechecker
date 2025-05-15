/**
 * @file rman.c
 * @author Jenny Vermeltfoort (jennyvermeltfoort@outlook.com)
 * https://github.com/jennyvermeltfoort
 * @brief Route manager.
 * @version 0.1
 * @date 2025-05-04
 * @copyright Copyright (c) 2025
 */

#include "rman.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/**
 * @brief Bereken met de pointer van een child in een struct de
 * pointer van de parent. Zorg ervoor dat de pointer naar de child ook
 * daadwerkelijk gealloceerd is binnen de verwachte parent.
 */
#define OFFSET_OF(type, member) ((size_t) & ((type*)0)->member)
#define CONTAINER_OF(ptr, type, member)                      \
    ({                                                       \
        const __typeof__(((type*)0)->member)* _mptr = (ptr); \
        (type*)((char*)_mptr - OFFSET_OF(type, member));     \
    })

#define TANK_CAPACITEIT_MIN 1    // Minimaal toegestane tank capaciteit.
#define TANK_CAPACITEIT_MAX 100  // Maximaal toegestane tank capaciteit
#define JOKERS_MAX 10            // Maximaal toegestane hoeveelheid jokers.
#define JOKER_MULT 0.5f          // Multiplier van een joker.
#define INF __INT_MAX__          // "Oneindig".

/**
 * @brief Generiek resultaat.
 */
typedef enum {
    RMAN_RES_OK = 0,
    RMAN_RES_ERR,
} rman_res_e;

/**
 * @brief De functie f_k.
 */
#pragma pack(1)
typedef struct {
#define F_I_GROOT (TANK_CAPACITEIT_MAX + 1)   // padding @ i = 0
#define F_J_GROOT (AANTAL_STATIONS_MAX + 2)   // padding @ j = 0 en + laatste station
#define F_K_GROOT (JOKERS_MAX + 2)            // padding @ k = 0 en + padding k-1 @ 0 < i.
    unsigned int p[F_J_GROOT][F_I_GROOT][1];  // padding zodat k-1 @ i = 0 oneindig is.
    unsigned int f[F_J_GROOT][F_I_GROOT][F_K_GROOT];
    // unsigned zodat INF + waarde geen rollover doet. 2^32 - 2^31 = 2^31, aangenomen dat j * prijs
    // niet groter is als een 32 bit signed int.
} rman_f_t;
#pragma pack()

/**
 * @brief Route manager.
 */
typedef struct {
    rman_handle_t handle;
    int tc;                           // tank capaciteit
    int sx[AANTAL_STATIONS_MAX + 2];  // station x coordinaat
    int sc[AANTAL_STATIONS_MAX + 2];  // station capaciteit
    int sp[AANTAL_STATIONS_MAX + 2];  // station prijs
    int c[AANTAL_STATIONS_MAX + 2];   // capaciteit te tanken bij station
    int a[AANTAL_STATIONS_MAX + 2];   // afstand afgelegd bij station.
    rman_f_t f;                       // De functie f, zie verslag.
    int sj[AANTAL_STATIONS_MAX][TANK_CAPACITEIT_MAX + 1]
          [JOKERS_MAX + 1];  // De functie h_j, zie verslag.
    bool sk[AANTAL_STATIONS_MAX][TANK_CAPACITEIT_MAX + 1]
           [JOKERS_MAX + 1];  // De functie h_k, zie verslag.
} rman_t;

inline bool is_nummer(char c) { return ('0' <= c && c <= '9'); }

inline int min(int a, int b) { return a * (a < b) + b * (b <= a); }

inline unsigned int umin(unsigned int a, unsigned int b) { return a * (a < b) + b * (b <= a); }

inline unsigned int umax(unsigned int a, unsigned int b) { return a * (a > b) + b * (b >= a); }

inline unsigned int cceil(float x) {
    int y = (int)x;
    if (x == (float)y) {
        return y;
    }
    return y + 1;
}

/**
 * @brief Lees een nummer uit het bestand.
 * @param b Handle naar het bestand.
 * @param n Het gelezen nummer.
 * @return rman_res_e RMAN_RES_ERR wanneer het gelezen karakter geen nummer is, RMAN_RES_OK:
 * success.
 */
rman_res_e bestand_lees_volgend_nummer(FILE* b, int* n) {
    rman_res_e res = RMAN_RES_ERR;
    *n = 0;
    int neg = 1;
    char c = fgetc(b);

    if (c == '-') {
        neg = -1;
        c = fgetc(b);
    }

    while (!feof(b) && is_nummer(c)) {
        *n = *n * 10 + (c - '0');
        res = RMAN_RES_OK;
        c = fgetc(b);
    }

    *n *= neg;

    return res;
}

/**
 * @brief Lees een bestand waarin de puzzel is omschreven, zie het verslag voor de structuur.
 * @param r Een ongeinitialiseerde route manager.
 * @param b Handle naar het bestand.
 * @return rman_res_e RMAN_RES_ERR: bestand voldoet niet aan de structuur, RMAN_RES_OK: success.
 */
rman_res_e lees_bestand(rman_t* r, FILE* b) {
    if (bestand_lees_volgend_nummer(b, &r->tc) != RMAN_RES_OK && TANK_CAPACITEIT_MIN <= r->tc &&
        r->tc <= TANK_CAPACITEIT_MAX) {
        return RMAN_RES_ERR;
    }

    if (bestand_lees_volgend_nummer(b, &r->handle.as) != RMAN_RES_OK &&
        AANTAL_STATIONS_MIN <= r->handle.as && r->handle.as <= AANTAL_STATIONS_MAX + 1) {
        return RMAN_RES_ERR;
    }
    r->handle.as++;  // +laatste station

    int vx = -__INT_MAX__;  // vorige x.
    for (int i = 1; i < r->handle.as; i++) {
        if (bestand_lees_volgend_nummer(b, &r->sx[i]) != RMAN_RES_OK ||
            bestand_lees_volgend_nummer(b, &r->sc[i]) != RMAN_RES_OK ||
            bestand_lees_volgend_nummer(b, &r->sp[i]) != RMAN_RES_OK) {
            return RMAN_RES_ERR;
        }

        if (r->sc[i] < 0 || r->sp[i] < 0 || r->sx[i] <= vx) {
            return RMAN_RES_ERR;
        }

        vx = r->sx[i];
    }

    if (bestand_lees_volgend_nummer(b, &r->sx[r->handle.as]) != RMAN_RES_OK) {
        return RMAN_RES_ERR;
    }

    return RMAN_RES_OK;
}

/**
 * @brief Initializeer een route manager.
 * @param r Een ongeinitialiseerde route manager.
 * @return rman_res_e RMAN_RES_ERR: de puzzel is niet op te lossen, RMAN_RES_OK: success.
 */
rman_res_e init_man(rman_t* r) {
    r->sx[0] = r->sx[1];

    for (int i = 1; i < r->handle.as + 1; i++) {
        r->a[i] = r->sx[i] - r->sx[i - 1];
        r->c[i] = min(r->sc[i] + r->c[i - 1] - r->a[i], r->tc);
        if (r->c[i] < 0) {
            // Het station is niet te bereiken.
            return RMAN_RES_ERR;
        }
    }

    for (int i = 0; i < r->handle.as + 1; i++) {
        for (int j = 0; j < F_I_GROOT; j++) {
            for (int k = -1; k < F_K_GROOT; k++) {
                r->f.f[i][j][k] = INF;
            }
        }
    }
    r->f.f[0][0][0] = 0;

    return RMAN_RES_OK;
}

/**
 * @brief Zoek een oplossing met memoization en een top down orde.
 * @param r Een geinitializeerde route manager.
 * @param i Station i.
 * @param j Tankinhoud j.
 * @param k Jokers in gebruik k.
 * @return unsigned int De kosten van de goedkoopste route.
 */
unsigned int td(rman_t* r, int i, int j, int k) {
    int ld = r->c[i - 1] - r->a[i];
    int lg = r->sc[i];
    int a = r->a[i];
    int p = r->sp[i];
    int cv = r->c[i - 1];

    if (i == 0 || k == -1 || r->c[i] < j || r->f.f[i][j][k] != INF) {
        return r->f.f[i][j][k];
    }

    if (j <= lg && j <= ld) {
        unsigned int f1 = td(r, i - 1, a, k) + j * p;
        unsigned int fk1 = td(r, i - 1, a, k - 1) + cceil(j * p * JOKER_MULT);
        unsigned int f1m = umin(f1, fk1);
        unsigned int f3 = td(r, i - 1, a + j, k);
        unsigned int fk3 = td(r, i - 1, a + j, k - 1);
        unsigned int f3m = umin(f3, fk3);
        unsigned int f = umin(f1m, f3m);
        r->sj[i - 1][j][k] = (f == f1 || f == fk1) ? j : 0;
        r->sk[i - 1][j][k] = (f == fk1 || f == fk3);
        r->f.f[i][j][k] = f;
    } else if (lg < j && j <= ld) {
        unsigned int f3 = td(r, i - 1, a + j, k);
        unsigned int fk3 = td(r, i - 1, a + j, k - 1);
        unsigned int f3m = umin(f3, fk3);
        unsigned int f4 = td(r, i - 1, a + j - lg, k) + lg * p;
        unsigned int fk4 = td(r, i - 1, a + j - lg, k - 1) + cceil(lg * p * JOKER_MULT);
        unsigned int f4m = umin(f4, fk4);
        unsigned int f = umin(f3m, f4m);
        r->sj[i - 1][j][k] = (f == f3 || f == fk3) ? 0 : lg;
        r->sk[i - 1][j][k] = (f == fk3 || f == fk4);
        r->f.f[i][j][k] = f;
    } else if (ld < j && j <= lg) {
        unsigned int f1 = td(r, i - 1, a, k) + j * p;
        unsigned int fk1 = td(r, i - 1, a, k - 1) + cceil(j * p * JOKER_MULT);
        unsigned int f1m = umin(f1, fk1);
        unsigned int f2 = td(r, i - 1, cv, k) + (j - ld) * p;
        unsigned int fk2 = td(r, i - 1, cv, k - 1) + cceil((j - ld) * p * JOKER_MULT);
        unsigned int f2m = umin(f2, fk2);
        unsigned int f = umin(f1m, f2m);
        r->sj[i - 1][j][k] = (f == f1 || f == fk1) ? j : j - ld;
        r->sk[i - 1][j][k] = (f == fk1 || f == fk2);
        r->f.f[i][j][k] = f;
    } else if (lg < j && ld < j) {
        unsigned int f2 = td(r, i - 1, cv, k) + (j - ld) * p;
        unsigned int fk2 = td(r, i - 1, cv, k - 1) + cceil((j - ld) * p * JOKER_MULT);
        unsigned int f2m = umin(f2, fk2);
        unsigned int f4 = td(r, i - 1, a + j - lg, k) + lg * p;
        unsigned int fk4 = td(r, i - 1, a + j - lg, k - 1) + cceil(lg * p * JOKER_MULT);
        unsigned int f4m = umin(f4, fk4);
        unsigned int f = umin(f2m, f4m);
        r->sj[i - 1][j][k] = (f == f2 || f == fk2) ? j - ld : lg;
        r->sk[i - 1][j][k] = (f == fk2 || f == fk4);
        r->f.f[i][j][k] = f;
    }

    return r->f.f[i][j][k];
}

/**
 * @brief Vind de goedkoopste strategie voor een opgeloste puzzel, roep eerst rman_los_op_bu/td aan.
 * Schrijft de strategie naar de handle.
 * @param r Een geinitializeerde route manager.
 * @param i Station i.
 * @param j Tankinhoud j.
 * @param k Jokers in gebruik k.
 */
void strategie(rman_t* r, int i, int j, int k) {
    if (i == 0) {
        return;
    }

    r->handle.oj[i - 1] = r->sj[i - 1][j][k];
    r->handle.ok[i - 1] = r->sk[i - 1][j][k];
    strategie(r, i - 1, j - r->handle.oj[i - 1] + r->a[i], k - 1 * r->handle.ok[i - 1]);
}

void rman_print(const rman_handle_t* h) {
    rman_t* r = CONTAINER_OF(h, rman_t, handle);

    printf("Instantie:\n");
    printf("Tank capaciteit: %i, aantal stations: %i.\n", r->tc, r->handle.as - 1);
    printf("x\t\tcap\t\tliterprijs\n");
    for (int i = 1; i < r->handle.as + 1; i++) {
        printf("%i\t\t%i\t\t%i\n", r->sx[i], r->sc[i], r->sp[i]);
    }

    for (int i = 1; i < r->handle.as + 1; i++) {
        printf("%i,%i,%i\n", i, r->a[i], r->c[i]);
    }
}

int rman_los_op_td(const rman_handle_t* h, int jokers) {
    rman_t* r = CONTAINER_OF(h, rman_t, handle);
    int f = td(r, r->handle.as - 1, r->a[r->handle.as], jokers);
    strategie(r, r->handle.as - 1, r->a[r->handle.as], jokers);
    return f;
}

int rman_los_op_bu(const rman_handle_t* h, int jokers) {
    rman_t* r = CONTAINER_OF(h, rman_t, handle);
    for (int i = 1; i < r->handle.as; i++) {
        int ld = r->c[i - 1] - r->a[i];
        int lg = r->sc[i];
        int a = r->a[i];
        int p = r->sp[i];
        int cv = r->c[i - 1];

        for (int j = 0; j <= r->c[i]; j++) {
            for (int k = 0; k <= i && k <= jokers; k++) {
                // Ik gebruik hier de rekenkundige manier van boolean logica om te een veelfout aan
                // branch prediction misses te voorkomen. Mogelijk lastig te lezen, vandaar wat
                // extra comments. In principe valt te logica groot en deels samen met de top down
                // variant.

                // f1 -> minimaal meenemen, de rest tanken.
                // f2 -> maximaal meenemen, de rest tanken.
                // f3 -> maximaal meenemen, niet tanken.
                // f4 -> maximaal tanken, de rest meenemen.
                unsigned int f1 = r->f.f[i - 1][a][k] + j * p;
                unsigned int f2 = r->f.f[i - 1][cv][k] + (j - ld) * p;
                unsigned int f3 = r->f.f[i - 1][a + j][k];
                unsigned int f4 = r->f.f[i - 1][a + j - lg][k] + lg * p;

                // De varianten van f1 ... f4 met een joker ingezet.
                unsigned int fk1 = r->f.f[i - 1][a][k - 1] + cceil(j * p * JOKER_MULT);
                unsigned int fk2 = r->f.f[i - 1][cv][k - 1] + cceil((j - ld) * p * JOKER_MULT);
                unsigned int fk3 = r->f.f[i - 1][a + j][k - 1];
                unsigned int fk4 = r->f.f[i - 1][a + j - lg][k - 1] + cceil(lg * p * JOKER_MULT);

                // Bepaald het minimum tussen wel of geen joker inzetten.
                unsigned int f1m = umin(f1, fk1);
                unsigned int f2m = umin(f2, fk2);
                unsigned int f3m = umin(f3, fk3);
                unsigned int f4m = umin(f4, fk4);

                // De limieten waarbinnen j moet vallen om f1 ... f4, f1k ... f4k in te zetten.
                // Wanneer de tankcapaciteit van het vorige station wordt overschreden (ld < j) dan
                // kunnen we niets anders als tanken, Wanneer de tankcapaciteti van het huidige
                // station is overschreden kunnen we niet anders dan de reset meenemen. Hier is ook
                // nog een combinatie mogelijk: alleen tanken, alleen menemen, gedeeltelijk meenemen
                // + gedeeltelijk tanken, alles tanken + meer menemen,
                bool fl1 = (j <= lg && j <= ld);
                bool fl2 = (lg < j && j <= ld);
                bool fl3 = (ld < j && j <= lg);
                bool fl4 = (lg < j && ld < j);

                unsigned int f = umin(f1m, f3m) * fl1 + umin(f3m, f4m) * fl2 +
                                 umin(f1m, f2m) * fl3 + umin(f2m, f4m) * fl4;

                bool sl1 = (fl1 || fl3);
                bool sl2 = (fl3 || fl4);
                bool sl3 = (fl1 || fl2);
                bool sl4 = (fl2 || fl4);

                // De minimale waardes fxm kunnen mogelijk overlappen wanneer de optimale waarde
                // voor een bepaalde tankcapaciteit hetzelfde is als een tegenhanger. Hier f4m en
                // f2m; f2m en f1m; hebben overlap wanneer ze vallen binnen de limieten van slx.
                // slx omschrijft in principe de range waarbinnen j moet vallen, waarmee de limieten
                // van fxm bepaald worden. Door fxm gelijk te stellen aan f bepalen we de
                // hoeveelheid diesel wat er getankt is en of er een joker is ingezet (f = fxk).
                r->sj[i - 1][j][k] = j * sl1 * (f == f1m) + lg * sl4 * (f == f4m && f != f2m) +
                                     (j - ld) * sl2 * (f == f2m && f != f1m);
                r->sk[i - 1][j][k] =
                    (f == fk3) * sl3 + (f == fk1) * sl1 + (f == fk2) * sl2 + (f == fk4) * sl4;
                r->f.f[i][j][k] = f;
            }
        }
    }

    r->f.f[r->handle.as][0][jokers] = r->f.f[r->handle.as - 1][r->a[r->handle.as]][jokers];
    strategie(r, r->handle.as - 1, r->a[r->handle.as], jokers);
    return r->f.f[r->handle.as][0][jokers];
}

const rman_handle_t* rman_maak(const char* pad) {
    FILE* b = fopen(pad, "r");

    if (b == NULL) {
        return NULL;
    }

    rman_t* r = calloc(1, sizeof(rman_t));
    rman_handle_t* h = &r->handle;

    if (lees_bestand(r, b) != RMAN_RES_OK || init_man(r) != RMAN_RES_OK) {
        free(r);
        h = NULL;
    }

    fclose(b);

    return h;
}

void rman_verwoest(const rman_handle_t* h) {
    rman_t* r = CONTAINER_OF(h, rman_t, handle);
    free(r);
}
