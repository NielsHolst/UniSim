/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "weather.h"

using namespace UniSim;


namespace seed_emergence {

Weather::Weather(UniSim::Identifier name, QObject *parent)
    : WeatherFile(name, parent)
{
    setColumn("Tavg", 8);
    setColumn("SWPkPa", 9);
    new PullVariable<double>("SWP", &swp, this, "Soil water potential in MPa");
}

void Weather::initialize() {
    WeatherFile::initialize();
    Parameter<char> *separator = seekOneChild<Parameter<char>*>("separator");
    separator->setValue(',');
    Parameter<int> *headerLines = seekOneChild<Parameter<int>*>("headerLines");
    headerLines->setValue(1);
}

void Weather::update() {
    WeatherFile::update();
    swp = pullVariable<double>("SWPkPA")/1000.;
}

} //namespace

