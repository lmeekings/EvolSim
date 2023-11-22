#pragma once

#include <vector>
#include <string_view>
#include "trait.h"

using namespace std;

namespace EvolSim
{
    class Creature
    {
    public:
        //The creature is either a carnivore or herbivore.
        enum class Eater { carnivore, herbivore };
        //The creature is either male or female.
        enum class Sex { male, female };
        Creature(Eater eater, string_view speciesName, int size, int width, int height);
        ~Creature();
        int getId() { return id; }
        vector<int> getLocation() { return location; }
        string_view getSpeciesName() { return speciesName; }
        Eater getEater() { return eater; }
        Sex getSex() { return sex; }
        /* A formula to determine reproductive health of a creature, it takes damage into consideration
            and gives a penalty of 10% for each point of starvation.
        */
        double getReproductiveHealth() {
            return static_cast<double>(health) * currentHitpoints /
                maxHitpoints - ((size - satiety) * 10);
        }
        /**
         * Gets creature stats including bonuses from traits
        */
        int getStealth();
        int getSense();
        int getMobility();
        int getSize() { return size; }
        int getOffence();
        int getDefence();
        int getColdAdaptation();
        int getHeatAdaptation();
        int getAge() { return age; }
        int getHealth() { return health; }
        int getSatiety() { return satiety; }
        void incrementAge() { age++; }

        void setSatiety(int foodIn) { satiety = foodIn; }
        void setHealth(int healthModifier) { health += healthModifier; }
        bool compareCreature(Creature otherCreature);
        bool compareSpecies(Creature otherCreature);
        double getCombatStrength();
        void takeDamage(int damage);
        int getHitpoints() { return currentHitpoints; }
        int getMaxHitpoints() { return maxHitpoints; }
        vector<Trait> reproduce(Creature otherCreature);
        void setTraits(vector<Trait> traits) { traitList = traits; }
        vector<Trait> getTraits() { return traitList; }
        void move(int x, int y);
        void consume(int foliage);
        void consume(Creature& prey);
        void die() { currentHitpoints = 0; }
        bool isDetected();
        bool isPoisoned() { return poisoned; }
        void setPoisoned(bool pois) { poisoned = pois; }
        void setSpeciesID(int specID) { speciesID = specID; }
        int getSpeciesID() { return speciesID; }

    private:
        //Each creature has a unique id to identify them.
        static int ID;
        int id;
        int speciesID;
        //Base statistics of all creatures, can be improved with traits.
        const static int baseHitpoints{ 50 };
        const static int baseMobility{ 3 };
        const static int baseVision{ 3 };
        const static int baseStealth{ 0 };
        //List of the creatures traits.
        vector<EvolSim::Trait> traitList;
        //The current location of the creature, i.e. {2, 1} for 2 X and 1 Y
        vector<int> location;

        //Name of the species
        string_view speciesName;
        //Age refers to the amount of cycles a creature has survived, a creatures health will diminish with age.
        int age{ 0 };
        //The size of the creature, bigger size equals more hitpoints but harder to hide and needs to eat more.
        int size{ 3 };
        //How much the creature has eaten, excessive hunger will harm the creature
        int satiety{ 0 };
        int health{ 100 };
        int maxHitpoints{ 50 };
        int currentHitpoints{ 50 };
        Eater eater;
        Sex sex;
        bool poisoned = false;
    };
}