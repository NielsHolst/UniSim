/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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
public: 
    DayDegrees(UniSim::Identifier name, QObject *parent=0);
	//standard methods
    void initialize();

    // special methods
    double calcDailyTimeStep();

protected:
	// parameters
    double T0, Topt, Tmax, T;

    // models
    UniSim::Model *weather;
};

} //namespace
#endif
