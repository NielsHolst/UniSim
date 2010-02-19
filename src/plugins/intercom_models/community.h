/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_COMMUNITY
#define INTERCOM_COMMUNITY
#include <QObject>
#include <usbase/model.h>
#include "light_components.h"
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
        int timeStep, layerStep;
        double hour, sinb;
        Weather::Par par;
        LightComponents weightedAreaAboveLayer[5];
    };

    const State* statePtr() const;

private:
	// parameters

	// state
    State s;

    // models
    UniSim::Calendar *calendar;
    Weather *weather;
    QList<Plant*> plants;

    // methods
    void updateWeightedAreaAboveLayer();
    void updatePlantsByHours();
    void updatePlantsByLayers();
};

} //namespace
#endif
