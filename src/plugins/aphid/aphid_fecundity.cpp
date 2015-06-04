/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "aphid_fecundity.h"

using namespace std;
using namespace UniSim;

namespace aphid {
	
AphidFecundity::AphidFecundity(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, vegetativePlants, 0.); // Proportion of plants in vegetative stage [0;1]
    Input(double, pre_floweringPlants, 0.); // Proportion of plants in flowering stage [0;1]
    Input(double, floweringPlants, 0.); // Proportion of plants in flowering stage [0;1]
    Input(double, maturePlants, 0.); // Proportion of plants in mature stage [0;1]
    Input(double, parentDensity, 0.); // Density of aphid parents
    Input(double, parentDensityThreshold, 10000.); // Density of aphid parents at which fecundity gets zero
    Input(double, maxFecundity, 20.); // Maximum number of nymphs born (per parent per day-degree)
    Input(double, dayDegreeTimeStep, 0.); // Time step duration (day-degrees)
    Output(double, newBorns); // Number of newborn nymphs(per day)
}

void AphidFecundity::reset() {
    newBorns = 0.;
}

void AphidFecundity::update() {
    newBorns = (vegetativePlants + pre_floweringPlants + floweringPlants)*maxFecundity*parentDensity*dayDegreeTimeStep;
    if (parentDensity > parentDensityThreshold)
        newBorns = 0.;
}

} //namespace

