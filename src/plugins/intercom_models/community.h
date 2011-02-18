/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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
	Q_OBJECT
public: 
    Community(UniSim::Identifier name, QObject *parent=0);
    // standard methods
    void initialize();
    void reset();
    void update();

private:
    // methods
    const double* sumELAI();
    void updateLai();
    void updateEarlyGrowth();
    void accumulateLateGrowth();
    void updateLateGrowth();

    // parameters
    double earlyGrowthThreshold;

    // pull variables
    double _sumELAI[3], lightAbsorption, CO2Assimilation, grossProduction, lai;

    // links
    QList<Area*> areas;
    QList<Plant*> plants;
};

} //namespace
#endif
