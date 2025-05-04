// Enkele constantes voor implementatie TankPlanner

#ifndef ConstantesHVar  // voorkom dat dit bestand meerdere keren
#define ConstantesHVar  // ge-include wordt

using namespace std;

const int MinHoeveel = 1;  // minimale hoeveelheid diesel op station
const int MinPrijs = 1;    // minimale prijs voor een liter diesel
const int MaxTank = 100;   // maximale inhoud van je dieseltank
const int MaxN = 500;      // maximum aantal tankstations op je route
const int MaxNrJokers = 10;  // maximum aantal beschikbare jokers
const int DefaultKosten =
    -1;  // default kosten voor als er geen oplossing is

#endif
