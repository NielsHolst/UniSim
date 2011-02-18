/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/pull_variable.h>
#include "weather.h"

using namespace UniSim;

namespace beehive{

Weather::Weather(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    new PullVariable<double>("Tavg", &Tavg, this,
        "Daily average temperature, calculated as the average between daily minimum "
        "and maximum temperature");
}

void Weather::initialize() {
    records = seekOneChild<Model*>("records");
}

void Weather::reset() {
    updateTavg();
}

void Weather::update() {
    updateTavg();
}

void Weather::updateTavg() {
    Tavg = (records->pullVariable<double>("Tmin") +
            records->pullVariable<double>("Tmax"))/2.;
}

} //namespace

