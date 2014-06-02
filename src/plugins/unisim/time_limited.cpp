/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QString>
#include <usbase/model.h>
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include <usbase/utilities.h>
#include "publish.h"
#include "time_limited.h"

namespace UniSim{

PUBLISH(TimeLimited)

TimeLimited::TimeLimited(Identifier name, QObject *parent)
    : Integrator(name, parent)
{
    new Parameter<double>("maxTime", &maxTime, 30., this, "description");
}

void TimeLimited::initialize() {
    Integrator::initialize();
    time = seekOneChild<Model*>("time");
}

bool TimeLimited::nextStep()
{
    return time->pullValue<double>("total") >= maxTime;
}

} //namespace

