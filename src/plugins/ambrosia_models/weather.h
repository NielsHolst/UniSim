/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef GRAINSTORE_WEATHER
#define GRAINSTORE_WEATHER
#include <usbase/weather_file.h>

namespace ambrosia{

class Weather : public UniSim::WeatherFile
{
	Q_OBJECT
public: 
	Weather(UniSim::Identifier name, QObject *parent=0);
    // standard methods
	void update();

private:
    // state (in addition to columns from weather file)
    double Tavg;
};

} //namespace
#endif
