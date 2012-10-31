/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "mussel_growth_rate.h"

using namespace UniSim;

namespace mussel_bed {
	
MusselGrowthRate::MusselGrowthRate(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("intrinsicRate", &intrinsicRate, 0.05, this, "Intrinsic rate of increase (per day)");
    new Parameter<double>("carryingCapacity", &carryingCapacity, 20., this, "Carrying capacity (kg/m2)");
    new Parameter<double>("density", &density, 0., this, "Mussel density (kg/m2)");
    new Variable<double>("value", &value, this, "desc");
}

void MusselGrowthRate::initialize() {
    scales = seekChildren<Model*>("scale");
}

void MusselGrowthRate::reset() {
    value = 0.;
}

void MusselGrowthRate::update() {
    value = intrinsicRate*density*(1. - density/carryingCapacity);
    for (int i = 0; i < scales.size(); ++i) {
        value *= scales[i]->pullValue<double>("value");
    }
}


} //namespace

