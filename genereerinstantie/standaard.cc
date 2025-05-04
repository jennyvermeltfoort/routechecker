// Implementatie van standaard functies.

#include "standaard.h"

#include <cstdlib>  // voor rand
#include <iostream>
using namespace std;

//*************************************************************************

bool integerInBereik(const char *variabele, int waarde, int minWaarde,
                     int maxWaarde) {
    if (waarde >= minWaarde && waarde <= maxWaarde)
        return true;
    else {
        cout << variabele << "=" << waarde << ", maar moet in ["
             << minWaarde << "," << maxWaarde << "] liggen." << endl;
        return false;
    }

}  // integerInBereik

//*************************************************************************

bool integerInBereik(int waarde, int minWaarde, int maxWaarde) {
    if (waarde >= minWaarde && waarde <= maxWaarde)
        return true;
    else
        return false;

}  // integerInBereik

//*************************************************************************

bool integerPositief(const char *variabele, int waarde) {
    if (waarde > 0)
        return true;
    else {
        cout << variabele << "=" << waarde
             << ", maar moet positief zijn." << endl;
        return false;
    }

}  // integerPositief

//*************************************************************************

int vraagInteger(const char *variabele, int minWaarde,
                 int maxWaarde) {
    int waarde;
    bool gelukt = false;

    do {
        cout << "Geef " << variabele << ": ";
        cin >> waarde;
        if (integerInBereik(variabele, waarde, minWaarde, maxWaarde))
            gelukt = true;
    } while (!gelukt);

    return waarde;

}  // vraagInteger

//*************************************************************************

int randomGetal(int min, int max) {
    int bereik, r;

    bereik = max - min + 1;

    r = ((rand()) % bereik) + min;
    return r;

}  // randomGetal

//*************************************************************************

void genereerRandomPermutatie(int n, int *permutatie) {
    int r,  // random positie in rest van array
        tmp;

    for (int i = 0; i < n; i++) permutatie[i] = i;

    for (int i = 0; i < n; i++) {  // vul permutatie[i]
        r = randomGetal(i, n - 1);
        tmp = permutatie[i];
        permutatie[i] = permutatie[r];
        permutatie[r] = tmp;
    }

}  // genereerRandomPermutatie
