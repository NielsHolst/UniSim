/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "weather.h"

using namespace std;
using namespace UniSim;

namespace ambrosia{

Weather::Weather(UniSim::Identifier name, QObject *parent)
    : WeatherFile(name, parent)
{
    setColumn("Tavg", 5);
}


void Weather::update()
{
    WeatherFile::update();
}

} //namespace

