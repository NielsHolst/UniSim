/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/utilities.h>
#include <usbase/variable.h>
#include "photo_thermal_time.h"
#include "publish.h"

namespace UniSim{

PUBLISH(PhotoThermalTime)

PhotoThermalTime::PhotoThermalTime(UniSim::Identifier name, QObject *parent)
    : DayDegrees(name, parent)
{
}

void PhotoThermalTime::initialize()
{
    DayDegrees::initialize();
    calendar = seekOne<Model*>("calendar");
}

double PhotoThermalTime::calcDailyTimeStep()
{
    double step =  DayDegrees::calcDailyTimeStep();
    step *= calendar->pullValue<double>("dayLength")/24.;
    return step;
}

} //namespace

