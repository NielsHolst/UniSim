/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QString>
#include <usbase/model.h>
#include <usbase/pull_variable.h>
#include <usbase/utilities.h>
#include "time_limited.h"

namespace UniSim{
	
TimeLimited::TimeLimited(Identifier name, QObject *parent)
    : Integrator(name, parent)
{
    new PullVariable<double>("numRuns", &numRuns, this, "description");
}

void TimeLimited::initialize() {
    setParameter("maxTime", &maxTime, 30., "description");
    time = seekOneChild<Model*>("time", this);
}

void TimeLimited::resetRuns() {
    numRuns = 0.;
}

void TimeLimited::resetSteps() {
}

bool TimeLimited::nextRun() {
    numRuns += 1.;
    return numRuns == 1;
}

bool TimeLimited::nextStep()
{
    return time->pullVariable<double>("total") >= maxTime;
}

} //namespace

