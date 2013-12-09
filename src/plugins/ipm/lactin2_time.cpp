/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include <usbase/utilities.h>
#include "../unisim/physiological_time.h"
#include "lactin2_time.h"

using namespace UniSim;

namespace ipm{

Lactin2Time::Lactin2Time(UniSim::Identifier name, QObject *parent)
    : PhysiologicalTime(name, parent)
{
    new Parameter<double>("a", &a, 0.11, this, "Equation parameter");
    new Parameter<double>("b", &b, 45.0, this, "Equation parameter");
    new Parameter<double>("c", &c,  8.0, this, "Equation parameter");
    new Parameter<double>("d", &d, 40.0, this, "Equation parameter");
}

void Lactin2Time::initialize()
{
    PhysiologicalTime::initialize();
    weather = seekOne<Model*>("weather");
}

double Lactin2Time::calcDailyTimeStep()
{
    double T = weather->pullValue<double>("Tavg");

	if (T > d)
		step = 0.0;
	else
		step = exp(a * T) - exp(a*b - (b - T)/c);
	return step;
}

} //namespace
