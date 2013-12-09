/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include <usbase/utilities.h>
#include "../unisim/physiological_time.h"
#include "linear_time.h"

using namespace UniSim;

namespace ipm{

LinearTime::LinearTime(UniSim::Identifier name, QObject *parent)
    : PhysiologicalTime(name, parent)
{
    new Parameter<double>("a", &a, 0.01000000, this, "Equation parameter");
    new Parameter<double>("b", &b, 2.000000000, this, "Equation parameter");
    new Parameter<double>("c", &c, 5.000000000, this, "Equation parameter");
    new Parameter<double>("d", &c, 30.000000000, this, "Equation parameter");
}

void LinearTime::initialize()
{
    PhysiologicalTime::initialize();
    weather = seekOne<Model*>("weather");
}

double LinearTime::calcDailyTimeStep()
{
    double T = weather->pullValue<double>("Tavg");
	
    if ((T <  c) || (T > d))
      step = 0.0;
    else  
      step = a + b * T;

    if (step < 0.)
        step = 0.;
    return step;
}

} //namespace

