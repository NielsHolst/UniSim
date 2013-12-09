/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/utilities.h>
#include <usbase/variable.h>
#include "fecundity.h"

using namespace UniSim;

namespace ipm {
	
Fecundity::Fecundity(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<double>(Name(density), 100., "Density of reproduction population");
    addParameter<double>(Name(maxPerCapitaFecundity), 1., "Max. fecundity (per individual per time unit)");
    addParameter<double>(Name(timeStep), 1., "Time increment (time units)");
    addParameter<double>(Name(sdRatio), 1, "Supply/demand ratio [0;1]");
    addVariable<double>(Name(value), "Current value");
}

void Fecundity::reset() {
    value = 0.;
}

void Fecundity::update() {
    value = density*maxPerCapitaFecundity*timeStep*sdRatio;
}

} //namespace

