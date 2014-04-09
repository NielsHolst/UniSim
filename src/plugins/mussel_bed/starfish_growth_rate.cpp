/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

/* ##STARFISH GROWTH: calculates the total increment in biomass density for starfish as a function
   of food intake and temperature (respiration costs), experimentally determined*/

#include "starfish_growth_rate.h"

using namespace UniSim;

namespace mussel_bed {

StarfishGrowthRate::StarfishGrowthRate(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("supply", &supply, 7, this, "total amount of mussel flesh consumed g/m2");
    new Parameter<double>("stDensity", &stDensity, 0.3, this, "current starfish density g/m2");
    new Parameter<double>("stsize",&stsize, 10, this, "average individual size in g");
    new Parameter<double>("stnumber",&stnumber, 50, this, "density in numbers/m2");
    new Variable<double>("value", &value, this, "total increase in starfish biomass at step g/m2");
}


void StarfishGrowthRate::reset() {
    value = 0.;
}

void StarfishGrowthRate::update() {

    value = (0.030023+0.901106*(supply/stnumber)-(0.000995*stsize))*stnumber;
}

} //namespace
