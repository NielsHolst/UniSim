/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include <usbase/utilities.h>
#include "../unisim/physiological_time.h"
#include "fecundity5_time.h"

using namespace UniSim;

namespace ipm{

Fecundity5Time::Fecundity5Time(UniSim::Identifier name, QObject *parent)
    : PhysiologicalTime(name, parent)
{
    new Parameter<double>("a", &a, 62.73336239, this, "Equation parameter");
    new Parameter<double>("b", &b, 22.67327981, this, "Equation parameter");
    new Parameter<double>("c", &c, 0.057454418, this, "Equation parameter");
	new Parameter<double>("d", &d, 15.0,        this, "Equation parameter");
	new Parameter<double>("e", &e, 35.0,        this, "Equation parameter");
}

void Fecundity5Time::initialize()
{
    PhysiologicalTime::initialize();
    weather = seekOne<Model*>("weather");
}

double Fecundity5Time::calcDailyTimeStep()
{
    double T = weather->pullValue<double>("Tavg");
	step = a * exp(-0.5 * (log(T / b) / c) * (log(T / b) / c) ); 
	
	if ( (T < d) || (T > e) )
        step = 1.0e-6;
    
    return step;
}

} //namespace