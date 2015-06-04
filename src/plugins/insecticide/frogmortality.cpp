/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "frogmortality.h"
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (frogmortality)
	
frogmortality::frogmortality(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, currentPopulation, 0.);  //current population of eggs, tadpole, ....adult frog
    Input (double, mortalityRate, 1.);     //per cent of eggs, tadpole, ....adult frog dies per hour or day

    Output (double, mortality);            //number of eggs, tadpole, ....adult frog dies per day

}

void frogmortality::reset() {

    mortality = 0.;

}

void frogmortality::update() {

    mortality = (mortalityRate/100.)*currentPopulation;

}

} //namespace

