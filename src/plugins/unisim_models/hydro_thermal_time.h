/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_HYDRO_THERMAL_TIME
#define UNISIM_HYDRO_THERMAL_TIME
#include <QObject>
#include "day_degrees.h"

namespace UniSim{

class HydroThermalTime : public DayDegrees
{
	Q_OBJECT
public: 
    HydroThermalTime(UniSim::Identifier name, QObject *parent=0);
    // special methods
    virtual double calcDailyTimeStep();
protected:
    // parameters
    double swp0, swpTopt, Kt;
    // pull variables
    double swpThreshold;
};

} //namespace
#endif
