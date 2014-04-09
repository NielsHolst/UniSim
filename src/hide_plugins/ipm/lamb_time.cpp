/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include <usbase/utilities.h>
#include "../unisim/physiological_time.h"
#include "lamb_time.h"

using namespace UniSim;

namespace ipm{

LambTime::LambTime(UniSim::Identifier name, QObject *parent)
    : PhysiologicalTime(name, parent)
{
    new Parameter<double>("a", &a,  0.454, this, "Equation parameter");
    new Parameter<double>("b", &b, 30.893, this, "Equation parameter");
    new Parameter<double>("c", &c,  5.475, this, "Equation parameter");
    new Parameter<double>("d", &d, 40.000, this, "Equation parameter");
}

void LambTime::initialize()
{
    PhysiologicalTime::initialize();
    weather = seekOne<Model*>("weather");
}

double LambTime::calcDailyTimeStep()
{
    double T = weather->pullValue<double>("Tavg");
	step = a * exp( (-0.5) * ((T - b) / c) * ((T - b) / c) ) ;
	if (T > d)
		step = 0.0;
    return step;
}

} //namespace
