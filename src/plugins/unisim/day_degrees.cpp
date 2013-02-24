/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include <usbase/utilities.h>
#include "day_degrees.h"

namespace UniSim{

DayDegrees::DayDegrees(UniSim::Identifier name, QObject *parent)
    : PhysiologicalTime(name, parent)
{
    new Parameter<double>("T0", &T0, missingValue<double>(), this,
        "Lower temperature threshold for development (@Char{ring}C)");
    new Parameter<double>("Topt", &Topt, 100., this,
        "Optimum temperature for development (@Char{ring}C)");
    new Parameter<double>("Tmax", &Tmax, 100., this,
        "Upper temperature threshold for development (@Char{ring}C)");
    new Parameter<double>("T", &T, missingValue<double>(), this,
        "Ambient temperature (@Char{ring}C). Default is a missing value (@F {NA}) "
        "which creates an automatic reference to @F {weather[Tavg]}");
}

void DayDegrees::initialize()
{
    PhysiologicalTime::initialize();
    if (isMissingValue(T))
        weather = seekOne<Model*>("weather");
}

double DayDegrees::calcDailyTimeStep()
{
    double T = isMissingValue(DayDegrees::T) ? weather->pullValue<double>("Tavg")
                                             : DayDegrees::T;
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

