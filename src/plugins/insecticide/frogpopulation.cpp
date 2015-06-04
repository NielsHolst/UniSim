/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "frogpopulation.h"
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (frogpopulation)
	
frogpopulation::frogpopulation(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, mortality, 0.);  //number of eggs, tadpole, ....adult frog dies per hour or day
    Input (double, growth, 1.);     //number of eggs, tadpole, ....adult frog

    Output (double, population);

}

void frogpopulation::reset() {

    population = 0.;

}

void frogpopulation::update() {

    population = growth - mortality;

}

} //namespace

