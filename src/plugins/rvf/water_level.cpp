/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "water_level.h"

using namespace UniSim;

namespace rvf {
	
WaterLevel::WaterLevel(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("initialValue", &initialValue, 100., this, "desc");
    new Parameter<double>("dailyRainfall", &dailyRainfall, 2., this, "desc");
    new Parameter<double>("dailyLoss", &dailyLoss, 10., this, "desc");
    new Variable<double>("value", &value, this, "desc");
}

void WaterLevel::reset() {
    value = initialValue;
}

void WaterLevel::update() {
    value += dailyRainfall - dailyLoss;
    if (value < 0.)
        value = 0.;
}

} //namespace

