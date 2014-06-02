/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include <usbase/utilities.h>
#include "day_degrees.h"
#include "publish.h"

namespace UniSim{

PUBLISH(DayDegrees)

DayDegrees::DayDegrees(UniSim::Identifier name, QObject *parent)
    : PhysiologicalTime(name, parent)
{
    addParameter<double>(Name(T0), 0.,
        "Lower temperature threshold for development (@Char{ring}C)");
    addParameter<double>(Name(Topt), 100.,
        "Optimum temperature for development (@Char{ring}C)");
    addParameter<double>(Name(Tmax), 100.,
        "Upper temperature threshold for development (@Char{ring}C)");
    addParameterRef<double>(Name(T), "weather[Tavg]");
}

void DayDegrees::initialize()
{
    PhysiologicalTime::initialize();
}

double DayDegrees::calcDailyTimeStep()
{
    double step;
    if (T < T0)
        step = 0.;
    else if (T < Topt)
        step = T - T0;
    else if (T < Tmax)
        step = Tmax - T;
    else
        step = 0;
    return step;
}

} //namespace

