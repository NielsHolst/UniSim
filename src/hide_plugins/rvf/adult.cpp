/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#include "adult.h"

using namespace UniSim;

namespace rvf {

Adult::Adult(Identifier name, QObject *parent)
     : Model(name, parent)
{
    new Parameter<double>("waterLevel", &waterLevel, 0., this, "desc");
    new Parameter<double>("waterLevelThreshold", &waterLevelThreshold, 0., this, "desc");
    new Parameter<double>("sexRatio", &sexRatio, 0.5, this, "Proportion of females");
    new Parameter<double>("density", &density, 0., this, "Density of adults");
    new Parameter<double>("fecundity", &fecundity, 40., this, "Eggs laid per female per day");
    new Parameter<double>("proportionEggsInWater", &proportionEggsInWater, 0., this, "desc");
    new Variable<double>("eggsLaidInWater", &eggsLaidInWater, this, "Eggs laid in water");
    new Variable<double>("eggsLaidOnGround", &eggsLaidOnGround,this, "Eggs laid on dry ground");
}
void Adult::reset() {
    eggsLaidInWater = 0.;
    eggsLaidOnGround = 0.;
}
void Adult::update() {
//    eggsLaid = sexRatio*density*fecundity;
//    eggsLaidInWater = (waterLevel < waterLevelThreshold) ? 0. : fecundity;
//    eggsLaidInWater = eggsLaid*proportionEggsInWater;
//    eggsLaidOnGround = eggsLaid-eggsLaidInWater;
    eggsLaid = sexRatio*density*fecundity;
    eggsLaidOnGround = eggsLaid;
}

} //namespace
