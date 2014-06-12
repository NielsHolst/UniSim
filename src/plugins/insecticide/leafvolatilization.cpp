/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "leafvolatilization.h"
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (leafvolatilization)
	
leafvolatilization::leafvolatilization(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, inflow, 0.);
    Input (double, volatilizationrateconstant, 0.);
    Output (double, concentration);
    Output (double, outflow);
}

void leafvolatilization::reset() {
    concentration = 0.;
    outflow = 0.;
}

void leafvolatilization::update() {
    outflow = volatilizationrateconstant*concentration;
    concentration += inflow - outflow;

}

} //namespace

