/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_WEATHER_INTERFACE
#define INTERCOM_WEATHER_INTERFACE
#include "weather_file.h"


namespace intercom {

class WeatherInterface : public UniSim::WeatherFile
{
	//Q_OBJECT
public: 
    WeatherInterface(UniSim::Identifier name, QObject *parent=0);
    struct Par {
        double total, diffuse, direct;
    };

protected:
    // pull variables (in addition to columns from weather file)
    double Tavg, Tday, irradiation;
    Par par;

};

} //namespace
#endif
