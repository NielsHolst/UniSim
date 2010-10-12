/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "time_step_limited.h"

namespace UniSim{
	
TimeStepLimited::TimeStepLimited(Identifier name, QObject *parent)
    : Integrator(name, parent)
{
    new Parameter<int>("maxSteps", &maxSteps, 100, this, "description");
}

void TimeStepLimited::resetRuns() {
    doRun = true;
}

void TimeStepLimited::resetSteps() {
    numSteps = 0;
}

bool TimeStepLimited::nextRun() {
    if (doRun) {
        doRun = false;
        return true;
    }
    return false;
}

bool TimeStepLimited::nextStep()
{
    ++numSteps;
    return numSteps <= maxSteps;
}

} //namespace
