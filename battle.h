#pragma once

#include "creature.h"
class Battle
{
public:
    Battle(EvolSim::Creature& attacker, EvolSim::Creature& defender);
    ~Battle();
    bool flee(EvolSim::Creature& runner, EvolSim::Creature& opponent);
    int checkSpecial(EvolSim::Creature& attacker, EvolSim::Creature& defender, int damage);

private:
    int turnNumber{ 0 };
};