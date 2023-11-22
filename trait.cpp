/* This class represents a trait in the simulation
* Author: Lewis James Meekings
*/
#include "trait.h"

using namespace EvolSim;

/* Creates a trait, with offence and defence values for combat. Mobility, sense and stealth serve other purposes.
Heat adaption and cold adaption for surviving extreme climates. Special for special abilities.
*/
EvolSim::Trait::Trait(int offence, int defence, int mobility, int sense, int stealth,
    int heatAdaptation, int coldAdaption, Special special)
{
    this->offence = offence;
    this->defence = defence;
    this->mobility = mobility;
    this->sense = sense;
    this->stealth = stealth;
    this->heatAdaptation = heatAdaptation;
    this->coldAdaptation = coldAdaptation;
    this->special = special;
}

/* Returns whether the trait has a special ability.
*/
bool EvolSim::Trait::hasSpecial()
{
    if (special == Special::none)
    {
        return false;
    }
    else
    {
        return true;
    }
}

/*A list of traits I created for animals to evolve.
*/
EvolSim::Trait winged{ 2,1,3,0,0,0,0,EvolSim::Trait::Special::none };
EvolSim::Trait shell{ 0,3,-1,0,0,0,0,EvolSim::Trait::Special::shell };
EvolSim::Trait largeEars{ 0,0,0,1,0,4,0,EvolSim::Trait::Special::none };
EvolSim::Trait binocularVision{ 1,1,0,3,0,0,0,EvolSim::Trait::Special::none };
EvolSim::Trait paddedFeet{ 2,0,1,0,0,2,2,EvolSim::Trait::Special::none };
EvolSim::Trait hooves{ 0,1,4,0,0,0,0,EvolSim::Trait::Special::none };
EvolSim::Trait quills{ 1,3,0,0,0,0,0,EvolSim::Trait::Special::quills };
EvolSim::Trait venom{ 3,0,0,0,0,0,0,EvolSim::Trait::Special::venom };
EvolSim::Trait scales{ 0,3,0,0,0,4,1,EvolSim::Trait::Special::none };
EvolSim::Trait fur{ 0,2,0,0,0,1,4,EvolSim::Trait::Special::none };
EvolSim::Trait feathers{ 0,1,0,0,0,3,2,EvolSim::Trait::Special::none };
EvolSim::Trait owlFeathers{ 0,1,0,0,2,3,2,EvolSim::Trait::Special::none };
EvolSim::Trait thickHide{ 1,3,0,0,0,1,1,EvolSim::Trait::Special::none };
EvolSim::Trait fangs{ 4,1,0,0,0,0,0,EvolSim::Trait::Special::none };
EvolSim::Trait claws{ 4,1,0,0,0,0,0,EvolSim::Trait::Special::none };
EvolSim::Trait horn{ 3,3,0,0,0,0,0,EvolSim::Trait::Special::none };
EvolSim::Trait camo{ 2,2,0,0,4,0,0,EvolSim::Trait::Special::none };
EvolSim::Trait warningColor{ 0,0,0,0,0,0,0,EvolSim::Trait::Special::warning };
EvolSim::Trait fat{ 0,2,0,0,0,-1,4,EvolSim::Trait::Special::none };

const vector<EvolSim::Trait>
EvolSim::Trait::universalTraitList = { winged, shell, largeEars, binocularVision, paddedFeet,
                                         hooves, quills, venom, scales, fur, feathers,
                                         stealthFeathers, thickHide, fangs, claws, horn, camo,
                                         warningColor, fat };
