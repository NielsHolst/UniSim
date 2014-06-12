/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "substance.h"
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (Substance)
	
Substance::Substance(Identifier name, QObject *parent)
	: Model(name, parent)
{
//    addParameter<double>(Name(inflow), 0., "Inflow of substance per day (g per day)");
//    addParameter<double>(Name(degradationConstant), 0., "Degradation rate per day");
//    addParameter<double>(Name(fractionformation), 0.995, "fractionformation of a metabolite");
//    addVariable<double>(Name(concentration), "Current concentration (g per day)");
//    addVariable<double>(Name(outflow), "Current outflow (g per day)");
    Input (double, inflow, 0.);
    Input (double, degradationConstant, 0.);
    Input (double, fractionformation, 0.995);
    Output (double, concentration);
    Output (double, outflow);
}

void Substance::reset() {
    concentration = 0.;
    outflow = 0.;
}

void Substance::update() {
    outflow = degradationConstant*concentration*fractionformation;
    concentration += inflow - outflow;

}

} //namespace

