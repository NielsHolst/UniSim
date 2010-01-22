/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_COMMUNITY
#define INTERCOM_COMMUNITY
#include <QObject>
#include <usbase/model.h>
#include "absorption_exponents.h"
#include "weather.h"

namespace UniSim {
    class Calendar;
};

namespace intercom{

class Plant;
class Weather;

class Community : public UniSim::Model
{
	Q_OBJECT
public: 
    Community(UniSim::Identifier name, QObject *parent=0);
	//standard methods
	void initialize();
	void reset();
	void update();

    // special methods
    struct State {
        double hour, sinb;
        Weather::Par par;
        AbsorptionExponents absorptionExponents;
    };

    const State* statePtr() const;

private:
	// parameters

	// state
    AbsorptionExponents absorptionExponents;
    State s;

    // models
    UniSim::Calendar *calendar;
    Weather *weather;
    QList<Plant*> plants;

    // methods
    void updatePlantsByHours();
    void updatePlantsByLayers(int time);
};

} //namespace
#endif
