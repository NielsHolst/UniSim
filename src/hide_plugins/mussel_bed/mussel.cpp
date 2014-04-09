/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.*/

/* ## MUSSEL POPULATION MODEL: actualize mussel population density at each step, considering the losses caused by thinning and predation
   and an environmentally scalated growth rate.*/
#include "mussel.h"

using namespace UniSim;

namespace mussel_bed {
	
Mussel::Mussel(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("initialDensity", &initialDensity, 3000., this, "Observed density at t0 as g/m2");
    new Parameter<double>("initialN", &initialN, 1000., this, "Observed density at t0 as numbers/m2");
    new Parameter<double>("LossB", &LossB, 0., this, "losses in mussel biomass as g/m2");
    new Parameter<double>("LossN", &LossN, 0., this, "losses in mussel numbers as g/m2");
    new Parameter<double>("growthRate", &growthRate, 0.3, this, "mussel growth rate (only in biomass)");
    new Variable<double>("N",&N,this,"current density at step, numbers/m2");
    new Variable<double>("density", &density, this, "current density at step, g/m2");
}

void Mussel::reset() {
    density = initialDensity;
    N = initialN;
}

void Mussel::update() {

    density += growthRate-LossB;
    N -= LossN;

if (density <0) /* to avoid problems if density gets below 0, temporal fix*/
    density=0;

}
} //namespace

