#pragma once

#include <vector>
using namespace std;
class Trait;

namespace EvolSim
{
    class Trait
    {
    public:
        const static vector<Trait> universalTraitList;
        /**
         * A collection of special abilities that some traits possess.
        */
        enum class Special { none, shell, warning, quills, venom };
        Trait(int offence, int defence, int mobility, int sense, int stealth, int heatAdaptation, int coldAdaptation, Special special);
        ~Trait();
        int getOffence() { return offence; }
        int getDefence() { return defence; }
        int getMobility() { return mobility; }
        int getSense() { return sense; }
        int getStealth() { return stealth; }
        int getHeatAdaptation() { return heatAdaptation; }
        int getColdAdaptation() { return coldAdaptation; }
        bool hasSpecial();
        Special getSpecial() { return special; }

    private:
        int offence;
        int defence;
        int mobility;
        int sense;
        int stealth;
        int heatAdaptation;
        int coldAdaptation;
        Special special;
    };
}