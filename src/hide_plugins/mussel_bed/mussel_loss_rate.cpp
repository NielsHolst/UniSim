/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "mussel_loss_rate.h"

using namespace UniSim;

namespace mussel_bed {

MusselLossRate::MusselLossRate(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("stdensity", &stdensity, 0.3, this, "Carrying capacity (kg/m2)");
    new Parameter<double>("feedingRate", &feedingRate, 0.117, this, "starfish feeding rate per day");
    new Variable<double>("value", &value, this, "desc");
}

void MusselLossRate::reset() {
    value = 0.;
}

void MusselLossRate::update() {
    value = stdensity*feedingRate;
   }

} //namespace
