/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/utilities.h>
#include <usbase/variable.h>
#include "demand.h"

using namespace UniSim;

namespace ipm {
	
Demand::Demand(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<double>(Name(density), 1., "Consumer density");
    addParameter<double>(Name(perCapitaDemand), 1., "Ressource needed (per consumer per time unit)");
    addParameter<double>(Name(timeStep), 1., "Time increment (time units)");
    addVariable<double>(Name(value), "Current value");
}

void Demand::reset() {
    value = 0;
}

void Demand::update() {
    value = density*perCapitaDemand*timeStep;
}

} //namespace

