/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include <usbase/pull_variable.h>
#include "physiological_time.h"

namespace UniSim{

PhysiologicalTime::PhysiologicalTime(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    new PullVariable<double>("step", &step, this,
        "Duration of latest time step (physiological time units)");
    new PullVariable<double>("total", &total, this,
        "Total duration since beginning of simulation (physiological time units)");
}

void PhysiologicalTime::initialize() {
    calendar = seekOne<Model*>("calendar");
    int stepsPerDay = calendar->parameter<int>("stepsPerDay");
    if (stepsPerDay <= 0)
        throw Exception("Calendar parameter 'stepPerDays' must be > 0");
    calendarTimeStep = 1./stepsPerDay;
}

void PhysiologicalTime::reset() {
    step = total = 0.;
}

void PhysiologicalTime::update() {
    step = calcDailyTimeStep()*calendarTimeStep;
    total += step;
}

} //namespace

