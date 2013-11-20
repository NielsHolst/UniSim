/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "logistic_growth.h"

using namespace UniSim;

namespace coral_reef {
	
LogisticGrowth::LogisticGrowth(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<double>(Name(initialDensity), 1., "Initial biomass density (kg/m2)");
    addParameter<double>(Name(carryingCapacity), 20., "Max. biomass density (kg/m2)");
    addParameter<double>(Name(growthRate), 0.01, "Growth rate (per day)");
    addParameter<int>(Name(timeStep), 1., "Time step (days)");
    addVariable<double>(Name(density), "Biomass density (kg/m2)");
}

void LogisticGrowth::reset() {
    density = initialDensity;
}

void LogisticGrowth::update() {
    double increment = density*growthRate*(carryingCapacity - density)/carryingCapacity*timeStep;
    density += increment;
    if (density < 0.)
        density = 0;
}

} //namespace

