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
    new Parameter<double>("sexRatio", &sexRatio, 0.5, this, "Proportion of females");
    new Parameter<double>("density", &density, 1., this, "Density of adults");
    new Parameter<double>("fecundity", &fecundity, 20., this, "Eggs laid per female per day");
    new Parameter<double>("proportionEggsInWater", &proportionEggsInWater, 20., this, "desc");
    new Variable<double>("eggsLaidInWater", &eggsLaidInWater, this, "Number of eggs laid in water");
    new Variable<double>("eggsLaidOnGround", &eggsLaidOnGround, this, "Number of eggs laid on dry ground");
}

void Adult::reset() {
    eggsLaidInWater = 0.;
    eggsLaidOnGround = 0.;
}

void Adult::update() {
    double eggsLaid = sexRatio*density*fecundity;
    eggsLaidInWater = eggsLaid*proportionEggsInWater;
    eggsLaidOnGround = eggsLaid-eggsLaidInWater;
}
} //namespace
