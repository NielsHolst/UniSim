/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "viscosityofwater.h"
#include <cmath>
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (viscosityofwater)
	
viscosityofwater::viscosityofwater(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, Tair, 25.);     //average air temperature in the month of application, degrees celsius

    Output (double, viscosity);    //viscosity of water (centipoise), between 0 - 50 degrees celsius
}

void viscosityofwater::reset() {

    viscosity = 0.;

}

void viscosityofwater::update() {

    double x = 1.337370563567217 + Tair * 0.001705494588353515;
    double y = 1. + Tair *0.01848546386356825;
    viscosity = std::pow(x/y, 2);

}

} //namespace

