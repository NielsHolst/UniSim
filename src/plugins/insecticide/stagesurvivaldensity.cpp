/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "stagesurvivaldensity.h"
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (stagesurvivaldensity)
	
stagesurvivaldensity::stagesurvivaldensity(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, inflow, 0.);  //current number of eggs, tadpole, ....adult frog
    Input (double, mortalityRate, 1.);     //per cent of eggs, tadpole, ....adult frog dies per hour or day

    Output (double, currentDensity);            //current number of eggs, tadpole, ....adult frog dies per day
    Output (double, outflow);

}

void stagesurvivaldensity::reset() {

    //currentDensity = 0.;
    outflow = 0.;

}

void stagesurvivaldensity::update() {

    //currentDensity = inflow;
    outflow = (1. - mortalityRate/100.)*inflow;

}

} //namespace

