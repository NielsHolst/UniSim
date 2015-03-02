/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/ustime.h>
#include "physiological_time.h"

namespace UniSim{

PhysiologicalTime::PhysiologicalTime(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Variable<double>("step", &step, this,
        "Duration of latest time step (physiological time units)");
    new Variable<double>("total", &total, this,
        "Total duration since beginning of simulation (in physiological time units) "
        "or since most recent trigger event (see e.g. @F TriggerByDate)");
    Input(bool, doReset, false);
    Input(bool, isTicking, true);
}

void PhysiologicalTime::initialize() {
    calendar = seekOne<Model*>("calendar");
    calendarTimeStep = calendar->pullValue<int>("timeStep");
    Time::Unit unit = Time::charToUnit( calendar->pullValue<char>("timeUnit") );
    calendarTimeStep *= Time::conversionFactor(unit, Time::Days);

    static bool always = true;
    Model *trigger = peekOneChild<Model*>("trigger");
    triggered = trigger ? trigger->pullValuePtr<bool>("value") : &always;
}

void PhysiologicalTime::reset() {
    step = total = 0.;
}

void PhysiologicalTime::update() {
    if (doReset) {
        step = total = 0.;
    }
    if (isTicking) {
        if (*triggered) {
            step = calcDailyTimeStep()*calendarTimeStep;
            total += step;
        }
        else {
            step = total = 0.;
        }
    }
}

} //namespace

