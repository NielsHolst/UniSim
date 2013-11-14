/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#ifndef RVF_EGG_TIME_H
#define RVF_EGG_TIME_H

#include "../unisim/day_degrees.h"

namespace rvf {

class EggTime : public UniSim::DayDegrees
{
public:
    EggTime(UniSim::Identifier name, QObject *parent);
    double calcDailyTimeStep();

private:
    // Parameters
    double waterLevel, waterLevelThreshold;

};

} //namespace

#endif // RVF_EGG_H

