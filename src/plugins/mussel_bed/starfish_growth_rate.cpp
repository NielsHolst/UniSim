/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "starfish_growth_rate.h"

using namespace UniSim;

namespace mussel_bed {

StarfishGrowthRate::StarfishGrowthRate(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("feedingRate", &feedingRate, 0.117, this, "mussel consuming rate");
    new Variable<double>("value", &value, this, "desc");
}


void StarfishGrowthRate::reset() {
    value = 0.;
}

void StarfishGrowthRate::update() {
    value = 0.2*feedingRate;
}

} //namespace
