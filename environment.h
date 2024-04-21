#pragma once

#include <vector>
#include "creature.h"
#include "battle.h"
using namespace std;
class Creature;

namespace EvolSim
{
    class Environment
    {
    public:
        Environment(int width, int height, double foliageChance,
            int foliageSize, int temperature, vector<Creature> speciesList);
        ~Environment();
        void initializeSpecies(vector<Creature> speciesList);
        void buildEnvironment();
        void startCycle();
        void resetEnvironment();
        void triggerDesertification();

        vector<int> searchFood(Creature creature);
        Creature searchPrey(Creature creature);
        vector<vector<int>> getSearchRegion(Creature creature);
        bool die(EvolSim::Creature c);
        void breed(Creature c);

        int getWidth() { return width; }
        int getHeight() { return height; }
        vector<vector<int>> getGridGraphics();
        vector<vector<int>> getCreatureGraphics();
        vector<EvolSim::Creature> getCreatureList() { return creatureList; }
        int getLiveCreatures() { return liveCreatures; }
        int getDeadCreatures() { return deadCreatures; }
        int getBornCreatures() { return bornCreatures; }
        int getTemperature() { return temperature; }
        int getNumSpecies() { return speciesList.size(); }
        string getSpeciesNames();

    private:
        //An array containing the original grid.
        vector<vector<int>> originalTileArray;
        //An array representing the 2d space of the simulation.
        vector<vector<int>> tileArray;
        //An array representing all the creatures contained in this environment.
        vector<Creature> creatureList;
        //An array containing each species
        vector<Creature> speciesList;
        //A baseline temperature that is neither extremely hot or extremely cold.
        const static int normalTemp{ 20 };

        //Dimensional variables of the simulation
        int width{ 10 };
        int height{ 10 };
        //The temperature of the simulation.
        int temperature{ 20 };
        //The current cycle the simulation is doing
        int cycleCount{ 0 };
        int liveCreatures;
        int deadCreatures;
        int bornCreatures;
        double foliageChance;
        int foliageSize;
    };
}