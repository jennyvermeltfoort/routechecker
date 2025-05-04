// Programma om random instantie te genereren voor derde
// programmeeropdracht Algoritmiek, voorjaar 2025: Goedkoop Tanken
//
// Rudy van Vliet, 29 april 2025

#include <cstdlib>  // voor srand
#include <fstream>
#include <iostream>

#include "constantes.h"
#include "standaard.h"
using namespace std;
const int MaxBestandsNaamLengte =
    30;  // maximale lengte van een bestandsnaam

//*************************************************************************

// Genereer n-1 random posities uit eersteX+1, ..., laatsteX-1,
// en sla deze op in posities[0..n-2].
// Station 0 komt op eersteX te liggen.
// Pre:
// * n <= MaxN
// * laatsteX - eersteX >= n, zodat we daadwerkelijk n-1 integers
//   kunnen vinden in eersteX+1, ..., laatsteX-1
void bepaalPosities(int posities[2 * MaxN], int n, int eersteX,
                    int laatsteX) {
    int nrMogPosities, i, j, tmp, pos;
    bool gevonden;

    nrMogPosities = laatsteX - eersteX - 1;
    if (nrMogPosities <=
        2 * MaxN)  // ze passen in ons hulparray posities
    {
        for (i = 0; i < nrMogPosities; i++)
            posities[i] = eersteX + 1 + i;

        for (i = 0; i < n - 1;
             i++)  // kies random positie uit de resterende posities
        {
            j = randomGetal(i, nrMogPosities - 1);
            tmp = posities[i];
            posities[i] = posities[j];
            posities[j] = tmp;
        }
    } else  // ze passen niet in ons hulparray;
            // ga dan maar net zo lang gokken tot je weer een nieuwe
            // positie hebt
    {
        for (i = 0; i < n - 1; i++) {
            do {
                pos = randomGetal(eersteX + 1, laatsteX - 1);
                gevonden = false;
                for (j = 0; j < i && !gevonden; j++)
                    if (posities[j] ==
                        pos)  // helaas, pos komt al voor
                        gevonden = true;
            } while (gevonden);
            posities[i] = pos;
        }
    }

}  // bepaalPosities

//*************************************************************************

// Sorteer de posities in posities[0..n-2] (selectionsort).
void sorteerPosities(int posities[2 * MaxN], int n) {
    int i, j, min, minj;

    for (i = 0; i < n - 2; i++) {
        min = posities[i];
        minj = i;
        for (j = i + 1; j < n - 1; j++) {
            if (posities[j] < min) {
                min = posities[j];
                minj = j;
            }
        }
        posities[minj] = posities[i];
        posities[i] = min;
    }

}  // sorteerPosities

//*************************************************************************

// Genereer een random instantie voor onze tankplanner, met de
// meegegeven parameters, en schrijf die naar tekstbestand fuit.
//
// Er worden n stations op verschillende posities tussen
// eersteX en laatsteX gegenereerd (inclusief eersteX, exclusief
// laatsteX), elk met een hoeveelheid tussen (en inclusief) MinHoeveel
// en maxHoeveel en een prijs tussen (en inclusief) MinPrijs en
// maxPrijs.
//
// Pre: controle van de parameters heeft al plaatsgevonden, dus er
// valt
//   daadwerkelijk een instantie te genereren.
void genereerRandomInstantie(ofstream &fuit, int inhoud, int n,
                             int eersteX, int laatsteX,
                             int maxHoeveel, int maxPrijs) {
    int posities[2 * MaxN],
        // mogelijke random posities voor stations 1..n-1
        i, pos, hoeveel, prijs;

    fuit << inhoud << endl;
    fuit << n << endl;

    // intelligente random seed
    srand(inhoud + n + eersteX + laatsteX + maxHoeveel + maxPrijs);

    bepaalPosities(posities, n, eersteX, laatsteX);
    sorteerPosities(posities, n);

    // de posities van onze stations hebben we;
    // nu nog de hoeveelheden en prijzen genereren
    for (i = 0; i < n; i++) {
        if (i == 0)
            pos = eersteX;
        else
            pos = posities[i - 1];

        hoeveel = randomGetal(MinHoeveel, maxHoeveel);
        prijs = randomGetal(MinPrijs, maxPrijs);

        fuit << pos << ' ' << hoeveel << ' ' << prijs << endl;
    }
    fuit << laatsteX << endl;

}  // genereerRandomInstantie

//*************************************************************************

// Genereer een random instantie.
// Vraag de gebruiker eerst de waardes van enkele parameters,
// en de naam van het tekstbestand waarin de instantie wordt
// geschreven.
void genereerinstantie() {
    int inhoud, eersteX, laatsteX, maxn, n, maxHoeveel, maxPrijs;
    char uitvoernaam[MaxBestandsNaamLengte + 1];
    ofstream fuit;

    inhoud = vraagInteger("inhoud tank", 1, MaxTank);

    cout << endl;
    cout << "Geef startpositie: ";
    cin >> eersteX;
    do {
        cout << "Geef eindpositie, groter dan " << eersteX << ": ";
        cin >> laatsteX;
    } while (laatsteX <= eersteX);

    maxn = laatsteX - eersteX;  // meer stations kun je niet kwijt
    if (maxn > MaxN) maxn = MaxN;
    n = vraagInteger("aantal stations n", 1, maxn);

    do {
        cout << "Geef maximale hoeveelheid diesel op een station, "
                "groter gelijk "
             << MinHoeveel << ": ";
        cin >> maxHoeveel;
    } while (maxHoeveel < MinHoeveel);

    do {
        cout << "Geef maximale prijs, groter gelijk " << MinPrijs
             << ": ";
        cin >> maxPrijs;
    } while (maxPrijs < MinPrijs);

    cout << "Geef de naam van het tekstbestand voor de instantie: ";
    cin >> uitvoernaam;
    fuit.open(uitvoernaam, ios::out);

    if (fuit.is_open()) {
        genereerRandomInstantie(fuit, inhoud, n, eersteX, laatsteX,
                                maxHoeveel, maxPrijs);
        fuit.close();
    } else {
        cout << endl;
        cout << "We konden het bestand `" << uitvoernaam
             << "' niet openen." << endl;
    }

}  // genereerinstantie

//*************************************************************************

int main() {
    genereerinstantie();

    return 0;
}
