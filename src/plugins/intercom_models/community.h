/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_COMMUNITY
#define INTERCOM_COMMUNITY
#include <QObject>
#include <usbase/model.h>
#include "weather.h"

namespace UniSim {
    class Calendar;
};

namespace intercom{

class Area;
class Photosynthesis;
class Plant;

class Community : public UniSim::Model
{
	Q_OBJECT
public: 
    Community(UniSim::Identifier name, QObject *parent=0);
	//standard methods
	void initialize();
	void reset();
	void update();


private:
	// parameters

	// state
    double grossLightAbsorption, grossAssimilation;

    // models
    Photosynthesis *photosynthesis;
    QList<Plant*> plants;
    QList<Area*> areas;
};

} //namespace
#endif
