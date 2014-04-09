/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

/* ## STARFISH POPULATION MODEL: actualize starfish population density at each step, considering the losses caused by salinity
   and an environmentally scalated growth rate.*/

#include "starfish.h"
#include "cmath"

using namespace UniSim;

namespace mussel_bed {

Starfish::Starfish(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("inDensity", &inDensity, 0., this, "Starfish density at t0 (g/m2");
    new Parameter<double>("mortality", &mortality, 0., this, "Starfish mortality at step (g)");
    new Parameter<double>("stgrowthRate", &stgrowthRate, 0.12, this, "Starfish population increase in biomass (g) at current step");
    new Variable<double>("stdensity", &stdensity, this, "current step starfish density (g/m2)");
}
void Starfish::reset() {
    stdensity = inDensity;
}

void Starfish::update() {
    double x = stgrowthRate - (stdensity*mortality);
    if (x <= 0 && abs(x)>=stdensity)
        stdensity = 0;
    else
    stdensity += stgrowthRate - (stdensity * mortality);
}


} //namespace

