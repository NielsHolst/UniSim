/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "temperature_scale.h"

using namespace UniSim;

namespace mussel_bed {
	
TemperatureScale::TemperatureScale(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("optimum", &optimum, 20., this, "desc");
    new Parameter<double>("temperature", &temperature, 12., this, "desc");
    new Variable<double>("value", &value, this, "desc");
}

void TemperatureScale::reset() {
    value = 1;
}

void TemperatureScale::update() {
    value = 1. - fabs(temperature - optimum)/optimum;
    if (value < 0.)
        value = 0.;
}


} //namespace

