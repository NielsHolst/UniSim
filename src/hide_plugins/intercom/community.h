/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_COMMUNITY
#define INTERCOM_COMMUNITY
#include <QObject>
#include <usbase/model.h>

namespace UniSim {
    class Model;
}

namespace intercom{

class Area;
class Plant;

class Community : public UniSim::Model
{
	//Q_OBJECT
public: 
    Community(UniSim::Identifier name, QObject *parent=0);
    // standard methods
    void initialize();
    void reset();
    void update();

private:
    // methods
    void updateSumELAI();
    void updateLai();

    // parameters
    double earlyGrowthThreshold;
    bool testMode;

    // pull variables
    double sumELAI[3], lightAbsorption, CO2Assimilation, grossProduction,
        maintenanceResp, growthResp, netAllocation,
        lai;

    // links
    QList<Area*> areas;
    QList<Plant*> plants;

    // data
    bool isEarly;
};

} //namespace
#endif
