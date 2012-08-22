/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef BEEHIVE_WEATHER
#define BEEHIVE_WEATHER
#include "weather_file.h"

namespace cotton{

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
