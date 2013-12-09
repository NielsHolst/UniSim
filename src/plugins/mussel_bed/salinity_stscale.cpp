/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

/* ##STARFISH FEEDING TEMPERATURE SCALING: this function scales the demand to the current
   temperature. From Ag�era et al. 2012*/

#include "salinity_stscale.h"
#include <math.h>
using namespace UniSim;

namespace mussel_bed {

SalinityStScale::SalinityStScale(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("smean", &smean, 28., this, "current temperature degree celsius");
    new Variable<double>("value", &value, this, "scaling factor for feeding rate");
}

void SalinityStScale::reset() {
    value = 1;
}

void SalinityStScale::update() {
    value = (exp(-4.62533+0.22619*smean)/(1+exp(-4.62533+0.22619*smean)));
    }

} //namespace
