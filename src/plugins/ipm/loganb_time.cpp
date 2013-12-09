/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include <usbase/utilities.h>
#include "../unisim/physiological_time.h"
#include "loganb_time.h"

using namespace UniSim;

namespace ipm{

LoganBTime::LoganBTime(UniSim::Identifier name, QObject *parent)
    : PhysiologicalTime(name, parent)
{
    new Parameter<double>("a", &a, 0.0093, this, "Equation parameter");
    new Parameter<double>("b", &b, 0.0959, this, "Equation parameter");
    new Parameter<double>("c", &c, 0.1142, this, "Equation parameter");
	new Parameter<double>("d", &d, 0.0000, this, "Equation parameter");
	new Parameter<double>("e", &e, 35.200, this, "Equation parameter");
}

void LoganBTime::initialize()
{
    PhysiologicalTime::initialize();
    weather = seekOne<Model*>("weather");
}

double LoganBTime::calcDailyTimeStep()
{
    double T = weather->pullValue<double>("Tavg");

    step = a*((exp( b*(T-d)))-(exp((b*(e-d))-((e-T)/c ))));
		
    if ((T < d) || (T > e))
        step = 0.0;
    return step;
}

} //namespace
