#include "creature.h"
#include "trait.h"
#include <vector>
#include <string_view>
#include <iostream>

using namespace EvolSim;
int Creature::ID = 0;

/* This is the constructor for a creature, it assigns it a food to 
*/
Creature::Creature(Eater eater, string_view speciesName, int size, int width, int height)
{
    int sexNum = rand() % 2;
    int changeSize = (rand() % 3) - 1;

    if (sexNum == 0)
    {
        sex = EvolSim::Creature::Sex::female;
    }
    else
    {
        sex = EvolSim::Creature::Sex::male;
    }
    this->eater = eater;
    this->speciesName = speciesName;
    if (size + changeSize > 0 && size + changeSize <= 5)
    {
        this->size = size + changeSize;
    }
    else if (size > 5)
    {
        this->size = 5;
    }
    else if (size < 1)
    {
        this->size = 1;
    }
    else
    {
        this->size = size;
    }
    id = ++ID;
    currentHitpoints = size * 25 + baseHitpoints;
    maxHitpoints = size * 25 + baseHitpoints;
    satiety = 0;
    age = 0;

    location = { rand() % width, rand() % height };
}
Creature::~Creature()
{

}

/**
 * Returns true if the other creature has the same id, which would mean they are the same creature.
*/
bool Creature::compareCreature(Creature otherCreature)
{
    return id == otherCreature.id;
}

/**
 * Determines if this creature has been found.
*/
bool Creature::isDetected()
{
    int hidingThreshold = 5 + (5 * getStealth());
    int detection = rand() % 101;

    if (detection > hidingThreshold)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Returns the defence of a creature which is determined by a sum of its traits.
*/
int Creature::getDefence()
{
    if (traitList.empty())
    {
        return 0;
    }
    else
    {
        int defence = 0;
        for (EvolSim::Trait t : traitList)
        {
            defence += t.getDefence();
        }
        return defence;
    }
}

/**
 * Returns the offence of a creature which is determined by a sum of its traits.
*/
int Creature::getOffence()
{
    if (traitList.empty())
    {
        return 0;
    }
    else
    {
        int offence = 0;
        for (EvolSim::Trait t : traitList)
        {
            offence += t.getOffence();
        }
        return offence;
    }
}

/**
 * Returns the stealth of a creature which is determined by a sum of its traits.
*/
int Creature::getStealth()
{
    if (traitList.empty())
    {
        return baseStealth;
    }
    else
    {
        int stealth = 0;
        for (EvolSim::Trait t : traitList)
        {
            stealth += t.getStealth();
        }
        return baseStealth + stealth;
    }
}

/**
 * Returns the extra mobility of a creature which is determined by a sum of its traits.
*/
int Creature::getMobility()
{
    if (traitList.empty())
    {
        return baseMobility;
    }
    else
    {
        int mobility = 0;
        for (EvolSim::Trait t : traitList)
        {
            mobility += t.getMobility();
        }
        return baseMobility + mobility;
    }
}

/**
 * Returns the extra vision of a creature which is determined by a sum of its traits.
*/
int Creature::getSense()
{
    if (traitList.empty())
    {
        return baseVision;
    }
    else
    {
        int sense = 0;
        for (EvolSim::Trait t : traitList)
        {
            sense += t.getSense();
        }
        return baseVision + sense;
    }
}

/**
 * Returns the cold adaptation of a creature which is determined by a sum of its traits.
*/
int Creature::getColdAdaptation()
{
    if (traitList.empty())
    {
        return 0;
    }
    else
    {
        int cold = 0;
        for (EvolSim::Trait t : traitList)
        {
            cold += t.getColdAdaptation();
        }
        return cold;
    }
}

/**
 * Returns the heat adaptation of a creature which is determined by a sum of its traits.
*/
int Creature::getHeatAdaptation()
{
    if (traitList.empty())
    {
        return 0;
    }
    else
    {
        int heat = 0;
        for (EvolSim::Trait t : traitList)
        {
            heat += t.getHeatAdaptation();
        }
        return heat;
    }
}

/**
 * Returns the combat strength of a creature that takes into account creature health, damage, size,
 * offensive and defensive traits and warning colors. Warning colors are worth a lot due to the value
 * of bluffing but won't help in a fight.
*/
double Creature::getCombatStrength()
{
    int warningPoints{ 0 };
    if (!traitList.empty())
    {
        for (Trait t : traitList)
        {
            if (t.hasSpecial() && t.getSpecial() == Trait::Special::warning)
            {
                warningPoints += 50;
            }
        }
    }

    warningPoints += 5 * size;
    return (currentHitpoints / maxHitpoints * health) + warningPoints + 10 * getOffence() + 10 * getDefence();
}

vector<EvolSim::Trait> Creature::reproduce(Creature otherCreature)
{
    vector<Trait> newCreatureTraits;
    if (!traitList.empty() || !otherCreature.traitList.empty())
    {
        if (!traitList.empty() && !otherCreature.traitList.empty())
        {
            int i = 0;
            for (Trait t : traitList)
            {
                int inheritance = rand() % 2;
                if (i < otherCreature.traitList.size())
                {
                    if (inheritance == 0)
                    {
                        newCreatureTraits.push_back(traitList[i]);
                    }
                    else
                    {
                        newCreatureTraits.push_back(otherCreature.traitList[i]);
                    }
                }
                else if (i >= otherCreature.traitList.size())
                {
                    if (inheritance == 0)
                    {
                        newCreatureTraits.push_back(traitList[i]);
                    }
                }
                i++;
            }

            if (i < otherCreature.traitList.size())
            {
                for (int j = i; j < otherCreature.traitList.size(); j++)
                {
                    int inheritance = rand() % 2;
                    if (inheritance == 0)
                    {
                        newCreatureTraits.push_back(otherCreature.traitList[i]);
                    }
                }
            }
        }

        if (!traitList.empty() && otherCreature.traitList.empty())
        {
            for (Trait t : traitList)
            {
                int inheritance = rand() % 2;
                if (inheritance == 0)
                {
                    newCreatureTraits.push_back(t);
                }
            }
        }

        if (!otherCreature.traitList.empty() && traitList.empty())
        {
            for (Trait t : traitList)
            {
                int inheritance = rand() % 2;
                if (inheritance == 0)
                {
                    newCreatureTraits.push_back(t);
                }
            }
        }
    }

    if (newCreatureTraits.size() < 5)
    {
        int mutateChance = rand() % 101;
        int newTrait = rand() % EvolSim::Trait::universalTraitList.size();
        if (mutateChance > 90)
        {
            newCreatureTraits.push_back(EvolSim::Trait::universalTraitList[newTrait]);
            //mutate new
        }
        else if (mutateChance > 80 && !newCreatureTraits.empty())
        {
            int replacedTrait = rand() % newCreatureTraits.size();
            newCreatureTraits.at(replacedTrait) = EvolSim::Trait::universalTraitList[newTrait];
        }
    }
    else if (newCreatureTraits.size() >= 5)
    {
        int mutateChance = rand() % 101;
        int newTrait = rand() % EvolSim::Trait::universalTraitList.size();
        if (mutateChance > 90 && !newCreatureTraits.empty())
        {
            int replacedTrait = rand() % newCreatureTraits.size();
            newCreatureTraits.at(replacedTrait) = EvolSim::Trait::universalTraitList[newTrait];
        }
    }

    return newCreatureTraits;
}

bool Creature::compareSpecies(Creature otherCreature)
{
    if (getSpeciesID() == otherCreature.getSpeciesID())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Creature::takeDamage(int damage)
{
    currentHitpoints -= std::max(0, damage);
}

void Creature::consume(int foliage)
{
    satiety += foliage;
}

void Creature::consume(Creature& prey)
{
    satiety += prey.getSize();
}

void Creature::move(int x, int y)
{
    location[0] = x;
    location[1] = y;
}