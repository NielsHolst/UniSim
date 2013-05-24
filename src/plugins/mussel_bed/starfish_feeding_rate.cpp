/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

/* ##STARFISH FEEDING RATE MODEL: determines the total demand of mussel flesh (no shell) for the current
   starfish density. This demand is scaled for the current temperature. Include respiration costs. Experimentally determined*/

#include "starfish_feeding_rate.h"

using namespace UniSim;

namespace mussel_bed {

StarfishFeedingRate::StarfishFeedingRate(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("stdensity",&stdensity, 0.3, this, "current starfish density Kg/m2");
    new Variable<double>("value", &value, this, "total demand for the current density kg/m2 of mussel flesh");
}

void StarfishFeedingRate::initialize() {
    scales = seekChildren<Model*>("stscale");
}

void StarfishFeedingRate::reset() {
    value = 0.;
}

void StarfishFeedingRate::update() {
    value = 0.02305*stdensity;
    for (int i = 0; i < scales.size(); ++i) {
        value *= scales[i]->pullValue<double>("value");
    }

}

} //namespace
