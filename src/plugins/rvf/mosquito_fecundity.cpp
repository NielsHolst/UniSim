/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "mosquito_fecundity.h"
#include "publish.h"

using namespace UniSim;

namespace rvf {

PUBLISH(MosquitoFecundity)

MosquitoFecundity::MosquitoFecundity(Identifier name, QObject *parent)
    : Model(name, parent)
{
    Input(double, waterLevel, 0.);
    Input(double, waterLevelThreshold, 0.);
    Input(double, fecundityPerBloodmeal, 40.); // Number of eggs laid per bloodmeal per female
    Input(double, sexRatio, 0.5);              // Proportion of females
    Input(double, density, 100.);              // Density of adults
    Input(double, bloodmeal, 0.2);             // Supply of blood meals per day
    Input(double, propInfectedEggs, 0.);      // Proportion infected eggs
    Output(double, infectedEggs);              // Number of infected eggs laid
    Output(double, unInfectedEggs);            // Number of uninfected eggs laid
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
