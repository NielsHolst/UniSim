/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/pull_variable.h>
#include "weather.h"

using namespace UniSim;

namespace beehive{

Weather::Weather(UniSim::Identifier name, QObject *parent)
    : WeatherFile(name, parent)
{
    new PullVariable<double>("Tavg", &Tavg, this,
        "Daily average temperature, calculated as average between daily minimum "
        "and maximum temperature");

    setColumn("Tmin", 2);
    setColumn("Tmax", 3);
}


void Weather::update() {
    WeatherFile::update();
    Tavg = (pullVariable<double>("Tmin") + pullVariable<double>("Tmax"))/2.;
}

} //namespace

