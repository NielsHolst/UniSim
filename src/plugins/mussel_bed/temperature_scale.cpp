/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

/* MUSSEL GROWTH SCALING: Scale maximum feeding rate by current step temperature,
   this scaling function is based in the data from van Haren & Kooijman, 1993
   (fig. 2, data from Sprung, 1984) */

#include "temperature_scale.h"
#include <math.h>
using namespace UniSim;

namespace mussel_bed {
	
TemperatureScale::TemperatureScale(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("temperature", &temperature, 12., this, "current step temperature in degree Celsius");
    new Parameter<double>("avgs", &avgs, 0.005, this, "average individual weight at current step");
    new Variable<double>("value", &value, this, "scaling parameter at current temperature");
}

void TemperatureScale::reset() {
    value = 1;
}

void TemperatureScale::update() {
    value = -1.7580306-0.3903995*log(avgs)+0.0894647*temperature;
    value = exp(value);
    }

} //namespace


