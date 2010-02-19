/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_AREA
#define INTERCOM_AREA
#include <QMap>
#include <QObject>
#include <usbase/model.h>
#include "light_components.h"
#include "community.h"
#include "light_use.h"

namespace UniSim {
    class Calendar;
}

namespace intercom{

class Area : public UniSim::Model
{
	Q_OBJECT
public: 
    typedef enum {Symmetric, Even, Tapering, TopHeavy} Distribution;

    Area(UniSim::Identifier name, QObject *parent=0);

	//standard methods
	void initialize();
	void reset();
	void update();

    // special methods
    LightComponents weightedAreaAboveLayer(double layerHeight);
    double atHeight(double height);
    double aboveHeight(double height);

private:
    // input management
    QMap<UniSim::Identifier, Distribution> lookupDist;
    QString distText;

    // parameters/state
    LightComponents k;

    // parameters
    int distribution;
    double initial, scatteringCoeff;

	// state
    double lai, amax, assimEfficiency, layerHeight;
    LightUse absorption, assimilation;

    // links
    UniSim::Calendar *calendar;
    UniSim::Model *weather, *plant, *plantHeight, *assimilationMax;
    const Community::State *cs;

    // methods
    void updateLightUseInShade();
    void updateLightUseInSun();
    void updateLightUseTotal();
};

} //namespace
#endif
