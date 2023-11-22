/* This class handles battles between multiple creatures. Such as between predator and prey.
* Author: Lewis James Meekings
*/
#include "creature.h"
#include "battle.h"
#include "trait.h"
#include <vector>
#include <string_view>
#include <iostream>

using namespace std;
using namespace EvolSim;

/* Creates a battle between 2 creatures
*/
Battle::Battle(EvolSim::Creature& attacker, EvolSim::Creature& defender)
{
    bool battlerFled{ false };
    int dice = { rand() % 101 };
    if (dice < 90)
    {
        battlerFled = flee(defender, attacker);
    }

    while (attacker.getHitpoints() > 0 && defender.getHitpoints() > 0 && !battlerFled)
    {
        if (dice >= 90 && turnNumber == 0)
        {
            int ambushDamage = { attacker.getOffence() - defender.getDefence() + rand() % 6 };
            defender.takeDamage(checkSpecial(attacker, defender, ambushDamage));
        }
        else
        {
            if ((defender.getEater() == EvolSim::Creature::Eater::herbivore and
                defender.getHitpoints() <= defender.getMaxHitpoints() / 5) or
                (defender.getEater() == EvolSim::Creature::Eater::carnivore and
                    defender.getHitpoints() <= defender.getMaxHitpoints() / 10 and
                    defender.getSatiety() >= defender.getSize()))
            {
                battlerFled = flee(defender, attacker);
            }
            if (attacker.getHitpoints() <= attacker.getMaxHitpoints() / 10 and
                defender.getSatiety() >= defender.getSize())
            {
                battlerFled = flee(attacker, defender);
            }
            int damage = { attacker.getOffence() - defender.getDefence() + rand() % 6 };
            defender.takeDamage(checkSpecial(attacker, defender, damage));

            if (defender.getHitpoints() > 0)
            {
                int damage = { defender.getOffence() - attacker.getDefence() + rand() % 6 };
                attacker.takeDamage(checkSpecial(defender, attacker, damage));
            }
        }
        turnNumber++;
        if (defender.isPoisoned())
        {
            defender.takeDamage(1);
        }
        if (attacker.isPoisoned())
        {
            attacker.takeDamage(1);
        }
    }
    if (attacker.getHitpoints() > 0 && defender.getHitpoints() <= 0)
    {
        attacker.consume(defender);
        attacker.move(defender.getLocation()[0], defender.getLocation()[1]);
    }

    if (defender.getHitpoints() > 0 && attacker.getHitpoints() <= 0
        && defender.getEater() == EvolSim::Creature::Eater::carnivore)
    {
        defender.consume(attacker);
    }
}

Battle::~Battle()
{

}

/* A creature tries to flee the battle.
*/
bool Battle::flee(EvolSim::Creature& runner, EvolSim::Creature& opponent)
{
    int fleeBonus{ 0 };
    if (turnNumber == 0)
    {
        fleeBonus = runner.getSense() - opponent.getStealth();
    }
    else
    {
        fleeBonus = runner.getMobility() - opponent.getMobility();
    }

    int dice = { rand() % 101 };

    if (dice + fleeBonus > 85)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/* Checks if the creatures have traits that can be used in combat.
*/
int Battle::checkSpecial(EvolSim::Creature& attacker, EvolSim::Creature& defender, int damage)
{
    int newDamage = damage;
    for (EvolSim::Trait t : attacker.getTraits())
    {
        switch (t.getSpecial())
        {
        case EvolSim::Trait::Special::venom:
            if (!defender.isPoisoned())
            {
                defender.setPoisoned(true);
            }
            break;
        case EvolSim::Trait::Special::quills:
        case EvolSim::Trait::Special::shell:
        case EvolSim::Trait::Special::none:
        case EvolSim::Trait::Special::warning:
            break;
        }
    }

    for (EvolSim::Trait t : defender.getTraits())
    {
        switch (t.getSpecial())
        {
        case EvolSim::Trait::Special::quills:
            attacker.takeDamage(1);
            break;
        case EvolSim::Trait::Special::shell:
            if (turnNumber == 0)
            {
                newDamage = 0;
            }
            break;
        case EvolSim::Trait::Special::venom:
        case EvolSim::Trait::Special::none:
        case EvolSim::Trait::Special::warning:
            break;
        }
    }
    return newDamage;
}