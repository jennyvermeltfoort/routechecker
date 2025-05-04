// Defintie van standaard functies.

#ifndef StandaardHVar  // om te voorkomen dat dit .h bestand meerdere
                       // keren
#define StandaardHVar  // wordt ge-include

// Controleer of variabele met naam `variabele' een waarde `waarde'
// heeft die tussen (inclusief) minWaarde en maxWaarde in ligt. Zo
// nee, geef een passende foutmelding.
//
// Voorbeeld van aanroep:
//   if (integerInBereik ("teller", teller, 0, 1000))
//     ...
bool integerInBereik(const char *variabele, int waarde, int minWaarde,
                     int maxWaarde);

// Controleer of waarde `waarde' tussen (inclusief) minWaarde en
// maxWaarde in ligt. Geef GEEN foutmelding als het niet zo is.
bool integerInBereik(int waarde, int minWaarde, int maxWaarde);

// Vraag de gebruiker om een integer op te geven tussen (inclusief)
// minWaarde en maxWaarde. Herhaal dit totdat daadwerkelijk zo'n
// integer is opgegeven.
int vraagInteger(const char *variabele, int minWaarde, int maxWaarde);

// Controleer of variabele met naam `variabele' een waarde `waarde'
// heeft die positief is. Zo nee, geef een passende foutmelding.
//
// Voorbeeld van aanroep:
//   if (integerPositief ("teller", teller))
//     ...
bool integerPositief(const char *variabele, int waarde);

// Genereer een random geheel getal r waarvoor  min <= r <= max.
// Pre: min <= max;
int randomGetal(int min, int max);

// Genereer een random permutatie van de getallen 0,1,2,...,n-1.
// Pre: n>=1 en array permutatie is groot genoeg voor n getallen.
void genereerRandomPermutatie(int n, int *permutatie);

#endif
