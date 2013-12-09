/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

/* ##STARFISH FEEDING TEMPERATURE SCALING: this function scales the demand to the current
   temperature. From Ag�era et al. 2012*/

#include "temperature_stscale.h"
#include <math.h>
using namespace UniSim;

namespace mussel_bed {

TemperatureStScale::TemperatureStScale(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("temperature", &temperature, 11., this, "current temperature degree celsius");
    new Variable<double>("value", &value, this, "scaling factor for feeding rate");
}

void TemperatureStScale::reset() {
    value = 1;
}

void TemperatureStScale::update() {
    value = (exp(0.853*temperature - 3.197)/(1+exp(0.853*temperature - 3.197)));
    }

} //namespace
