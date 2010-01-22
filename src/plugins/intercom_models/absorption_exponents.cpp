/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "plant.h"

namespace intercom{

AbsorptionExponents::AbsorptionExponents()
{
    for (int i = 0; i < NumLightComponents; ++i)
        exponents.append(0.);
}

void AbsorptionExponents::reset() {
    for (int i = 0; i < NumLightComponents; ++i)
        exponents[i] += 0.;
}

void AbsorptionExponents::accumulate(const AbsorptionExponents &increments) {
    for (int i = 0; i < NumLightComponents; ++i)
        exponents[i] += increments.value(i);
}

double AbsorptionExponents::value(LightComponent lc) const {
    return exponents.at(lc);
}

double AbsorptionExponents::value(int lc) const {
    return exponents.at(lc);
}

double& AbsorptionExponents::operator[] (int lc) {
    return exponents[lc];
}

double& AbsorptionExponents::operator[] (LightComponent lc) {
    return exponents[lc];
}

} //namespace

