/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/pull_variable.h>
#include "weather.h"

using namespace UniSim;


namespace dynamic_photosynthesis {

Weather::Weather(UniSim::Identifier name, QObject *parent)
        : WeatherFile(name, parent)
{
    setColumn("Temp", 1);
    setColumn("PPFD", 2);
}

} //namespace

