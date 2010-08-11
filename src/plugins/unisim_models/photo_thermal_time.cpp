/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/utilities.h>
#include "photo_thermal_time.h"

namespace UniSim{

PhotoThermalTime::PhotoThermalTime(UniSim::Identifier name, QObject *parent)
    : DayDegrees(name, parent)
{
}

void PhotoThermalTime::initialize()
{
    DayDegrees::initialize();
    calendar = seekOne<Model*>("calendar");
}

void PhotoThermalTime::update()
{
    double prevTotal = total;
    DayDegrees::update();
    step *= calendar->pullVariable<double>("dayLength")/24.;
    total = prevTotal + step;
}

} //namespace

