/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "substance.h"

using namespace UniSim;

namespace insecticide {
	
Substance::Substance(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<double>(Name(inflow), 0., "Inflow of substance per day");
    addParameter<double>(Name(degradationConstant), 0.1, "Degradation rate per day");
    addVariable<double>(Name(concentration), "Current concentration (unit?)");
    addVariable<double>(Name(outflow), "Current outflow (units? per day)");
}

void Substance::reset() {
    concentration = 0.;
    outflow = 0.;
}

void Substance::update() {
    outflow = degradationConstant*concentration;
    concentration += inflow - outflow;
}

} //namespace

