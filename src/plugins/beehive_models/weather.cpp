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
    new PullVariable("Tavg", &Tavg, this);
    setColumn("Tmin", 2);
    setColumn("Tmax", 3);
}


void Weather::update()
{
    WeatherFile::update();
    Tavg = (pullVariable("Tmin") + pullVariable("Tmax"))/2.;
}

} //namespace

