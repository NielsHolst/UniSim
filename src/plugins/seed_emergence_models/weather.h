/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef SEED_EMERGENCE_WEATHER
#define SEED_EMERGENCE_WEATHER
#include <QObject>
#include <usbase/weather_file.h>

namespace seed_emergence {

class Weather : public UniSim::WeatherFile
{
	Q_OBJECT
public: 
	Weather(UniSim::Identifier name, QObject *parent=0);
    // standard methods
    void initialize();
    void update();
private:
    // pull variables
    double swp;
};

} //namespace
#endif
