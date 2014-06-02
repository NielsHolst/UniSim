/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "logistic_growth.h"

using namespace UniSim;

namespace example {
	
LogisticGrowth::LogisticGrowth(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<double>(Name(initialDensity), 5., "Initial population density (per km2)");
    addParameter<double>(Name(growthRate), 0.01, "Growth rate (per DD)");
    addParameter<double>(Name(carryingCapacity), 500., "Max. population density (per km2)");
    addParameter<double>(Name(timeStep), 1., "Time step (DD)");
    addVariable<double>(Name(density), "Population density (per km2)");
}

void LogisticGrowth::reset() {
    density = initialDensity;
}

void LogisticGrowth::update() {
    double increment = density*growthRate*(carryingCapacity - density)/carryingCapacity*timeStep;
    density += increment;
}

} //namespace

