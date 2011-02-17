/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_WEATHER
#define INTERCOM_WEATHER
#include <usbase/model.h>
#include "weather_interface.h"

namespace UniSim {
    class Calendar;
}

namespace intercom {

    class Weather : public UniSim::Model
{
	Q_OBJECT
public: 
    Weather(UniSim::Identifier name, QObject *parent=0);
    // standard methods
    void initialize();
    void reset();
    void update();

    struct Par {
        double total, diffuse, direct;
    };

private slots:
    void handleClockTick(double hour);

private:
    // methods
    void updatePar();

    // pull variables (in addition to columns from records file)
    double Tavg, Tday, irradiation;
    Par par;

    // links
    UniSim::Calendar *calendar;
    UniSim::Model *records;

};

} //namespace
#endif
