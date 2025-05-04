// Definitie van klasse TankPlanner.

#ifndef TankPlannerHVar  // om te voorkomen dat dit .h bestand
                         // meerdere keren
#define TankPlannerHVar  // wordt ge-include

#include <utility>
#include <vector>

#include "constantes.h"
extern "C" {
#include "rman.h"
}

class TankPlanner {
   public:
    // Default constructor
    TankPlanner();

    // Destructor
    ~TankPlanner();

    // Lees een instantie in vanuit een door de gebruiker opgegeven
    // tekstbestand.
    // Controleer daarbij of het een geldige instantie is:
    // * of de parameters c en n binnen de grenzen van de opdracht
    // vallen
    // * of alle hoeveelheden diesel en de prijzen positief zijn
    // * of alle posities (van de pompstations en de bestemming)
    //   verschillend en oplopend zijn
    // Pre:
    // * De parameter n bovenin het tekstbestand klopt met de regels
    // met
    //   getallen daaronder. Dat wil zeggen: daaronder staan
    //   daadwerkelijk n regels met drie integers, gevolgd door een
    //   laatste regel met een integer.
    // Post:
    // * Als het een geldige instantie is, is de invoer opgeslagen in
    //   membervariabelen.
    // Retourneer:
    // * true, als het inderdaad een geldige instantie is.
    // * false, anders
    bool leesIn(const char *invoernaam);

    // Druk de instantie af op het scherm.
    // Controleer eerst of er een geldige instantie is.
    void drukAfInstantie();

    // Bepaal een optimale oplossing voor de huidige instantie,
    // met behulp van top-down dynamisch programmeren,
    // als er tenminste een oplossing mogelijk is.
    // Maak gebruik van maximaal nrJokers jokers.
    // Controleer eerst of nrJokers binnen de grenzen van de opdracht
    // valt en of er een geldige instantie is. Retourneer:
    // * de kosten van een optimale oplossing, als
    //   - nrJokers binnen de grenzen van de opdracht valt, en
    //   - er een geldige instantie is, en
    //   - er een oplossing voor deze instantie bestaat
    // * -1, anders
    int losOpTD(int nrJokers);

    // Bepaal een optimale oplossing voor de huidige instantie,
    // met behulp van bottom-up dynamisch programmeren,
    // als er tenminste een oplossing mogelijk is.
    // Maak gebruik van maximaal nrJokers jokers.
    // Controleer eerst of nrJokers binnen de grenzen van de opdracht
    // valt en of er een geldige instantie is. Retourneer:
    // * de kosten van een optimale oplossing, als
    //   - nrJokers binnen de grenzen van de opdracht valt, en
    //   - er een geldige instantie is, en
    //   - er een oplossing voor deze instantie bestaat
    // * -1, anders
    // Post:
    // * Als er een oplossing gevonden is, bevat parameter strategie
    //   een optimale tankstrategie, met per tankstation
    //   (0,1,2,...,n-1, in die volgorde):
    //   - de hoeveelheid diesel die getankt wordt en
    //   - een boolean die true is als er een joker wordt gebruikt en
    //     false als er geen joker wordt gebruikt
    // * Als er geen oplossing gevonden is, is strategie leeg
    int losOpBU(int nrJokers, vector<pair<int, bool> > &strategie);

   private:
    // TODO: uw eigen memberfuncties en membervariabelen
    const rman_handle_t *rman_handle = nullptr;
};

#endif
