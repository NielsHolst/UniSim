/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "mosquito_fecundity.h"

using namespace UniSim;

namespace rvf {

MosquitoFecundity::MosquitoFecundity(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("waterLevel", &waterLevel, 0., this, "desc");
    new Parameter<double>("waterLevelThreshold", &waterLevelThreshold, 0., this, "desc");
    new Parameter<double>("fecundityPerBloodmeal", &fecundityPerBloodmeal, 40., this, "Number of eggs laid per bloodmeal per female");
    new Parameter<double>("sexRatio", &sexRatio, 0.5, this, "Proportion of females");
    new Parameter<double>("density", &density, 100., this, "Density of adults");
    new Parameter<double>("bloodmeal", &bloodmeal, 0.2, this, "Supply of blood meals per day");
    new Parameter<double>("propInfectedEggs", &propInfectedEggs, 0., this, "Proportion infected eggs");
    new Variable<double>("infectedEggs", &infectedEggs, this, "Infected Eggs");
    new Variable<double>("uninfectedEggs", &unInfectedEggs, this, "desc");
}

void MosquitoFecundity::reset() {
    unInfectedEggs = 0;
    infectedEggs = 0;
}

void MosquitoFecundity::update() {
    double totalEggs = (waterLevel < waterLevelThreshold) ? 0. : fecundityPerBloodmeal*density*sexRatio*bloodmeal;
    unInfectedEggs = totalEggs * (1 - propInfectedEggs);
    infectedEggs = totalEggs * propInfectedEggs;
}

} //namespace
