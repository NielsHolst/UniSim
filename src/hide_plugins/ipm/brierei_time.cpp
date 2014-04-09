/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include <usbase/utilities.h>
#include "../unisim/physiological_time.h"
#include "brierei_time.h"

using namespace UniSim;

namespace ipm{

BriereITime::BriereITime(UniSim::Identifier name, QObject *parent)
    : PhysiologicalTime(name, parent)
{
    new Parameter<double>("a", &a, 0.000131734, this, "Equation parameter");
    new Parameter<double>("b", &b, 10.25740308, this, "Equation parameter");
    new Parameter<double>("c", &c, 36.65400490, this, "Equation parameter");
}

void BriereITime::initialize()
{
    PhysiologicalTime::initialize();
    weather = seekOne<Model*>("weather");
}

double BriereITime::calcDailyTimeStep()
{
    double T = weather->pullValue<double>("Tavg");
	step = a * T * (T - b) * sqrt(c - T);
    if (step < 0.)
        step = 0.;
    return step;
}

} //namespace

