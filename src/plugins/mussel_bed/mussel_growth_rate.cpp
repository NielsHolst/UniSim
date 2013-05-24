/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

/* ## MUSSEL GROWTH MODEL: calculate an increase of biomass for the mussel population, based in an
   intrinsic (optimum) growth rate that is scalated by current temperature and salinity at step*/

#include "mussel_growth_rate.h"

using namespace UniSim;

namespace mussel_bed {
	
MusselGrowthRate::MusselGrowthRate(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("intrinsicRate", &intrinsicRate, 0.019, this, "Intrinsic rate of increase (% per day), maximum growth observed in the Wadden Sea for young mussels");
    new Parameter<double>("carryingCapacity", &carryingCapacity, 15., this, "Carrying capacity (kg/m2), obtained from field observations");
    new Parameter<double>("density", &density, 3., this, "current mussel density at step(kg/m2)");
    new Variable<double>("value", &value, this, "total growth rate for the current density, temperature and salinity (kg/m2)");
}

void MusselGrowthRate::initialize() {
    scales = seekChildren<Model*>("scale");
}

void MusselGrowthRate::reset() {
    value = 0.;
}

void MusselGrowthRate::update() {
    value = intrinsicRate*density*(1. - density/(carryingCapacity));
    for (int i = 0; i < scales.size(); ++i) {
        value *= scales[i]->pullValue<double>("value");
    }
}


} //namespace

