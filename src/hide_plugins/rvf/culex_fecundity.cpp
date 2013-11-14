/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "culex_fecundity.h"

using namespace UniSim;

namespace rvf {
	
CulexFecundity::CulexFecundity(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("waterLevel", &waterLevel, 0., this, "desc");
    new Parameter<double>("waterLevelThreshold", &waterLevelThreshold, 0., this, "desc");
    new Parameter<double>("dailyFecundity", &dailyFecundity, 40., this, "Eggs laid per female per day");
    new Parameter<double>("sexRatio", &sexRatio, 0.5, this, "Proportion of females");
    new Parameter<double>("density", &density, 0., this, "Density of adults");
    new Variable<double>("value", &value, this, "desc");
}

void CulexFecundity::reset() {
    value = 0;
}

void CulexFecundity::update() {
    double eggsPerFemale = (waterLevel < waterLevelThreshold) ? 0. : dailyFecundity;
//    double eggsPerFemale = std::max(waterLevel/waterLevelThreshold, 1.)*dailyFecundity;
    value = eggsPerFemale*density*sexRatio;
}


} //namespace

