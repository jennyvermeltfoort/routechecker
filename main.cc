// Hoofdprogramma voor oplossing voor derde programmeeropdracht
// Algoritmiek, voorjaar 2025: Goedkoop Tanken
//
// Biedt de gebruiker een menustructuur om
// * een instantie in te lezen uit een bestand
// * een instantie TD op te lossen zonder jokers
// * een instantie TD op te lossen met jokers
// * een instantie BU op te lossen zonder jokers
// * een instantie BU op te lossen met jokers
//
// Namen studenten, datum

#include <iostream>

#include "standaard.h"
#include "tankplanner.h"
using namespace std;
const int MaxBestandsNaamLengte =
    30;  // maximale lengte van een bestandsnaam

//*************************************************************************

// Druk de tankstrategie in parameter `strategie' af op het scherm,
// met de bijbehorende kosten `kosten'.
void drukAfOplossing(int kosten, vector<pair<int, bool> > strategie) {
    int nrJokers,  // aantal jokers dat we gebruikt hebben
        grootte;   // grootte van vector strategie

    cout << endl;
    cout << "Een optimale strategie is:" << endl;
    nrJokers = 0;
    grootte = strategie.size();
    for (int i = 0; i < grootte; i++) {
        cout << "Tankstation " << i << ": " << strategie[i].first
             << " liter";
        if (strategie[i].second) {
            cout << " met joker" << endl;
            nrJokers++;
        } else
            cout << " zonder joker" << endl;
    }
    cout << endl;
    cout << "De kosten hiervan bedragen: " << kosten << endl;
    cout << "We hebben " << nrJokers << " jokers gebruikt." << endl;

}  // drukAfOplossing

//*************************************************************************

// Bied de gebruiker een menu aan en verwerk zijn keuze.
void hoofdmenu() {
    TankPlanner *tp1;  // pointer, om makkeijk nieuwe objecten te
                       // kunnen maken en weer weg te gooien
    int keuze, nrJokers, kosten;
    char invoernaam[MaxBestandsNaamLengte + 1];
    vector<pair<int, bool> > strategie;

    tp1 = new TankPlanner();

    do {
        cout << endl;
        cout << "1. Een instantie inlezen uit een bestand" << endl;
        cout << "2. De instantie TD oplossen zonder jokers" << endl;
        cout << "3. De instantie TD oplossen met jokers" << endl;
        cout << "4. De instantie BU oplossen zonder jokers" << endl;
        cout << "5. De instantie BU oplossen met jokers" << endl;
        cout << "6. Stoppen" << endl;
        cout << endl;
        cout << "Maak een keuze: ";
        cin >> keuze;
        switch (keuze) {
            case 1:
                cout << endl;
                cout << "Geef de naam van het tekstbestand met de "
                        "instantie: ";
                cin >> invoernaam;
                if (tp1->leesIn(invoernaam)) tp1->drukAfInstantie();
                break;
            case 2:
            case 3:
                if (keuze == 2)
                    nrJokers = 0;
                else
                    nrJokers =
                        vraagInteger("aantal jokers", 1, MaxNrJokers);
                kosten = tp1->losOpTD(nrJokers);
                cout << endl;
                if (kosten != DefaultKosten)
                    cout << "De minimale kosten bedragen " << kosten
                         << endl;
                else
                    cout << "Er is geen oplossing mogelijk." << endl;
                break;
            case 4:
            case 5:
                if (keuze == 4)
                    nrJokers = 0;
                else
                    nrJokers =
                        vraagInteger("aantal jokers", 1, MaxNrJokers);
                kosten = tp1->losOpBU(nrJokers, strategie);
                if (kosten != DefaultKosten)
                    drukAfOplossing(kosten, strategie);
                break;
            case 6:
                break;
            default:
                cout << endl;
                cout << "Voer een goede keuze in!" << endl;
        }

    } while (keuze != 6);

    delete tp1;

}  // hoofdmenu

//*************************************************************************

int main() {
    hoofdmenu();

    return 0;
}
