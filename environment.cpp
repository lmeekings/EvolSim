/* The environment class governs the space that creatures reside in, it can be made to be harsh or mild.
* Currently factors like food and temperature can be changed here.
* Author: Lewis James Meekings
*/
#include "environment.h"
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <algorithm>

using namespace std;
using namespace EvolSim;

/**
 * This is a constructor for environment it builds an environment using user selected criteria.
*/
Environment::Environment(int width, int height, double foliageChance,
    int foliageSize, int temperature, vector<Creature> speciesList)
{
    this->width = width;
    this->height = height;
    this->temperature = temperature;
    this->speciesList = speciesList;
    deadCreatures = 0;
    bornCreatures = 0;

    buildEnvironment(width, height, foliageChance, foliageSize);
    this->originalTileArray = this->tileArray;
    initializeSpecies(speciesList);
}
/**
 * This builds a grid for the environment which is required for the movement of creatures.
*/
void Environment::buildEnvironment(int width, int height, double foliageChance,
    int foliageSize)
{
    for (int i = 0; i < width; i++)
    {
        vector<int> temp;
        for (int j = 0; j < height; j++) {
            int size = 0;
            double random = (float)rand() / RAND_MAX;
            if (random <= foliageChance && foliageSize > 0)
            {
                size = 1 + (rand() % foliageSize);
            }
            temp.push_back(size);
        }
        tileArray.push_back(temp);
    }
}
/**
 * This is the destructor
*/
Environment::~Environment()
{
    this->tileArray.clear();
}

/**
 * Searches for food within the environment, if the creature is a herbivore it will look for foliage,
 * however if the creature is a carnivore it will look for prey.
*/
vector<int> Environment::searchFood(Creature creature)
{
    vector<vector<int>> searchRegion;
    searchRegion = getSearchRegion(creature);
    int maxFood = 0;
    int i = 0;
    int j = 0;

    for (vector<int> square : searchRegion)
    {
        if (tileArray[square[0]][square[1]] > maxFood)
        {
            maxFood = tileArray[square[0]][square[1]];
            j = i;
        }
        i++;
    }

    return searchRegion[j];
}

/**
 * The creature searches for nearby prey, large foliage obscures creatures.
*/
EvolSim::Creature Environment::searchPrey(Creature creature)
{
    vector<vector<int>> searchRegion = getSearchRegion(creature);
    vector<Creature> preyList;

    for (Creature c : creatureList)
    {
        if (count(searchRegion.begin(), searchRegion.end(), c.getLocation())
            && !creature.compareCreature(c) && c.getHitpoints() > 0)
        {
            if (c.isDetected())
            {
                preyList.push_back(c);
            }
        }
    }

    if (!preyList.empty()) {
        double minCombatStrength{ preyList[0].getCombatStrength() };
        int i = 0;
        int j = 0;
        for (Creature c : preyList)
        {
            if (c.getCombatStrength() < minCombatStrength)
            {
                minCombatStrength = c.getCombatStrength();
                j = i;
            }
            i++;
        }
        return preyList[j];
    }
    else
    {
        return creature;
    }
}

/**
 * Returns all the squares that a creature can reach this turn, its movement is limited by vision and mobility.
 * All directions of movement work but the creature is limited to the bounds of the environment.
*/
vector<vector<int>> Environment::getSearchRegion(Creature creature) {
    int locationX = creature.getLocation()[0];
    int locationY = creature.getLocation()[1];
    int searchRange;
    vector<vector<int>> searchingRegionPartial;
    vector<vector<int>> searchingRegionFull;
    searchRange = min(creature.getMobility(), creature.getSense());

    for (int i = 0; i <= searchRange; i++)
    {
        for (int j = 0; i + j <= searchRange; j++)
        {
            vector<int> temp = { i, j };
            searchingRegionPartial.push_back(temp);
        }
    }

    searchingRegionFull.push_back({ locationX, locationY });
    searchingRegionPartial.erase(searchingRegionPartial.begin());

    for (vector<int> k : searchingRegionPartial) {
        if (locationX + k[0] >= getWidth() || locationY - k[1] < 0) {}
        else
        {
            searchingRegionFull.push_back({ locationX + k[0], locationY - k[1] });
        }

        if (locationX - k[0] < 0 || locationY + k[1] >= getHeight()) {}
        else
        {
            searchingRegionFull.push_back({ locationX - k[0], locationY + k[1] });
        }
        if (k[0] > 0 and k[1] > 0)
        {
            if (locationX - k[0] < 0 || locationY - k[1] < 0) {}
            else
            {
                searchingRegionFull.push_back({ locationX - k[0], locationY - k[1] });
            }
            if (locationX + k[0] >= getWidth() || locationY + k[1] >= getHeight()) {}
            else
            {
                searchingRegionFull.push_back({ locationX + k[0], locationY + k[1] });
            }
        }
    }
    return searchingRegionFull;
}

/**
 * Resets the environment to its original state.
*/
void Environment::resetEnvironment()
{
    tileArray = originalTileArray;
}

/**
 * Initializes the creatures in this environment and assigns species IDs to identify species.
*/
void Environment::initializeSpecies(vector<EvolSim::Creature> speciesList)
{
    int speciesID = 0;
    for (EvolSim::Creature c : speciesList)
    {
        creatureList.push_back(c);
        c.setSpeciesID(speciesID);
        for (int i = 0; i < 19; i++)
        {
            EvolSim::Creature c2{ c.getEater(), c.getSpeciesName(), c.getSize(), getWidth(), getHeight() };
            c2.setSpeciesID(c.getSpeciesID());
            creatureList.push_back(c2);
        }
        speciesID++;
    }
    liveCreatures = creatureList.size();
}

/**
 * Each creature performs one action, and then reproduction and death events occur.
*/
void Environment::startCycle()
{
    if (cycleCount == 0) {}
    else
    {
        for (EvolSim::Creature c : creatureList)
        {
            c.setSatiety(0);
        }
    }
    liveCreatures = creatureList.size();
    for (int i = 0; i < creatureList.size();)
    {
        if (creatureList[i].getHitpoints() == 0)
        {

        }
        else if (creatureList[i].getEater() == Creature::Eater::herbivore)
        {
            vector<int> foodLocation = searchFood(creatureList[i]);
            if (tileArray[foodLocation[0]][foodLocation[1]] <= 0) {}
            else if (tileArray[foodLocation[0]][foodLocation[1]] >= creatureList[i].getSize())
            {
                tileArray[foodLocation[0]][foodLocation[1]] -= creatureList[i].getSize();
                creatureList[i].consume(creatureList[i].getSize());
            }
            else
            {
                creatureList[i].consume(tileArray[foodLocation[0]][foodLocation[1]]);
                tileArray[foodLocation[0]][foodLocation[1]] = 0;
            }
            creatureList[i].move(foodLocation[0], foodLocation[1]);
        }
        else
        {
            Creature predator = creatureList[i];
            Creature prey = searchPrey(predator);
            if (!predator.compareCreature(prey))
            {
                Battle(predator, prey);
            }
        }

        if (creatureList[i].getHitpoints() <= 0 or die(creatureList[i]))
        {
            creatureList.erase(creatureList.begin() + i);
        }
        else
        {
            creatureList[i].incrementAge();
            ++i;
        }
    }

    deadCreatures = liveCreatures - creatureList.size();

    if (!creatureList.empty())
    {
        for (int i = 0; i < creatureList.size(); i++)
        {
            breed(creatureList[i]);
        }
    }

    bornCreatures = creatureList.size() - (liveCreatures - deadCreatures);

    cycleCount++;
    resetEnvironment();
}


/**
 * Handles creature reproduction
*/
void Environment::breed(Creature creature)
{
    int breedingChance = rand() % 101;
    if (creature.getSex() == EvolSim::Creature::Sex::female
        and breedingChance + (creature.getSize() - creature.getSatiety() * 20) < 10)
    {
        vector<vector<int>> mateLocations = getSearchRegion(creature);
        vector<EvolSim::Creature> mateList;

        for (EvolSim::Creature c : creatureList)
        {
            if (count(mateLocations.begin(), mateLocations.end(), c.getLocation())
                and !creature.compareCreature(c) and c.getSex() == EvolSim::Creature::Sex::male
                and creature.compareSpecies(c))
            {
                mateList.push_back(c);
            }
        }
        if (!mateList.empty()) {
            int reproductiveHealth = mateList[0].getReproductiveHealth();
            EvolSim::Creature chosenMate = mateList[0];
            for (EvolSim::Creature c : mateList)
            {
                if (c.getReproductiveHealth() > reproductiveHealth)
                {
                    chosenMate = c;
                }
            }
            int babies = 1 + rand() % 4;
            for (int i = 1; i <= babies; i++)
            {
                int newSize = ((creature.getSize() + chosenMate.getSize())) / 2;
                EvolSim::Creature newborn{ creature.getEater(), creature.getSpeciesName(), newSize, getWidth(), getHeight() };
                newborn.setTraits(creature.reproduce(chosenMate));
                newborn.setSpeciesID(creature.getSpeciesID());
                creatureList.push_back(newborn);
            }
        }
    }
}

/**
 * Determines non-combat death. Creatures have a small chance to randomly die,
 * but the probability increases due to age and extreme temperatures.
*/
bool Environment::die(EvolSim::Creature c)
{
    int chanceToDie = rand() % 101;
    int extremeWeatherModifier{ 0 };
    int creatureDamage = (static_cast<double> (1) /
        static_cast<double> (c.getHitpoints()) / c.getMaxHitpoints()) + 0.5;
    c.setHealth(-creatureDamage);
    if (c.isPoisoned())
    {
        c.setHealth(-5);
        c.setPoisoned(false);
    }
    int healthDamage = 100 - c.getHealth();

    if (temperature > normalTemp + 5)
    {
        extremeWeatherModifier = (temperature - normalTemp + 5) - c.getHeatAdaptation();
    }
    else if (temperature < normalTemp - 5)
    {
        extremeWeatherModifier = ((normalTemp - 5) - temperature) - c.getColdAdaptation();
    }
    if (extremeWeatherModifier < 0)
    {
        extremeWeatherModifier = 0;
    }
    if (chanceToDie + ((c.getSize() - c.getSatiety()) * 20) +
        extremeWeatherModifier + (c.getAge() * 5) + healthDamage >= 99)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/* Returns an array representing the environment which is used to draw the grid
*/
vector<vector<int>> Environment::getGridGraphics()
{
    vector<vector<int>> newVector = tileArray;
    return newVector;
}

/* Returns an array showing where the creatures are on the grid.
It returns the density of creatures per grid square used to represent the creatures graphically.
*/
vector<vector<int>> Environment::getCreatureGraphics()
{
    vector<vector<int>> creatureVector;
    for (int i = 0; i < width; i++)
    {
        vector<int> tempVector;
        for (int j = 0; j < height; j++)
        {
            int creatureCount = std::count_if(creatureList.begin(), creatureList.end(), [i, j](EvolSim::Creature x)
                                {return x.getLocation()[0] == i and x.getLocation()[1] == j; });
            tempVector.push_back(creatureCount);
        }
        creatureVector.push_back(tempVector);
    }
    return creatureVector;
}

/* Returns a list of all the species in this environment.
*/
string Environment::getSpeciesNames()
{
    string temp = "Species names:\n";
    for (Creature c : speciesList)
    {
        temp = temp + static_cast<string>(c.getSpeciesName()) + "\n";
    }

    return temp;
}