/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_DAY_DEGREES
#define UNISIM_DAY_DEGREES
#include <QObject>
#include <usbase/model.h>
#include "physiological_time.h"

namespace UniSim{

class DayDegrees : public PhysiologicalTime
{
	Q_OBJECT
public: 
    DayDegrees(UniSim::Identifier name, QObject *parent=0);
	//standard methods
    virtual void initialize();

    // special methods
    virtual double calcDailyTimeStep();

protected:
	// parameters
    double T0, Topt, Tmax;

    // models
    UniSim::Model *weather;
};

} //namespace
#endif
