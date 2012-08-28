/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include <usbase/utilities.h>
#include "lactin_time.h"

namespace UniSim{

LactinTime::LactinTime(UniSim::Identifier name, QObject *parent)
    : PhysiologicalTime(name, parent)
{
    new Parameter<double>("a", &a, 0.13, this, "Equation parameter");
    new Parameter<double>("b", &b, 42., this, "Equation parameter");
    new Parameter<double>("c", &c, 8., this, "Equation parameter");
    new Parameter<double>("d", &d, -0.1, this, "Equation parameter");
}

void LactinTime::initialize()
{
    weather = seekOne<Model*>("weather");
}

double LactinTime::calcDailyTimeStep()
{
    double T = weather->pullValue<double>("Tavg");
    double step = exp(a*T) - exp(a*b - (b - T)/c) + d;
    if (step < 0.)
        step = 0.;
    return step;
}

} //namespace

