/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include <usbase/utilities.h>
#include "../unisim/physiological_time.h"
#include "cuadratic_long_time.h"

using namespace UniSim;

namespace ipm{

CuadraticLongTime::CuadraticLongTime(UniSim::Identifier name, QObject *parent)
    : PhysiologicalTime(name, parent)
{
    new Parameter<double>("a", &a, 0.000873474, this, "Equation parameter");
    new Parameter<double>("b", &b, 8.01471e-05, this, "Equation parameter");
    new Parameter<double>("c", &c, 13.0,  this, "Equation parameter");
	new Parameter<double>("d", &d, 35.0,  this, "Equation parameter");
}

void CuadraticLongTime::initialize()
{
    PhysiologicalTime::initialize();
    weather = seekOne<Model*>("weather");
}

double CuadraticLongTime::calcDailyTimeStep()
{
    double T = weather->pullValue<double>("Tavg");
	double maxs = a * c + b * d * d;
    double step;
	
	if ((T < c) || (T > d))
        step = maxs;
    else  
        step = a * T + b * T*T;

    return step;
}

} //namespace

