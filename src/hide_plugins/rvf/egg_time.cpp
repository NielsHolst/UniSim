/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "../unisim/day_degrees.h"
#include "egg_time.h"

using namespace UniSim;

namespace rvf {

EggTime::EggTime(Identifier name, QObject *parent)
     : DayDegrees(name, parent)
{
    new Parameter<double>("waterLevel", &waterLevel, 0., this, "desc");
    new Parameter<double>("waterLevelThreshold", &waterLevelThreshold, 0., this, "desc");
}

double EggTime::calcDailyTimeStep() {
    return (waterLevel < waterLevelThreshold) ? 0. : DayDegrees::calcDailyTimeStep();
}

} //namespace
