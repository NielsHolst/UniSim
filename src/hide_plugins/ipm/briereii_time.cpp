/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include <usbase/utilities.h>
#include "../unisim/physiological_time.h"
#include "briereii_time.h"

using namespace UniSim;

namespace ipm{

BriereIITime::BriereIITime(UniSim::Identifier name, QObject *parent)
    : PhysiologicalTime(name, parent)
{
    new Parameter<double>("a", &a, 0.0000494135, this, "Equation parameter");
    new Parameter<double>("b", &b, 11.91223575, this, "Equation parameter");
    new Parameter<double>("c", &c, 40.60000534, this, "Equation parameter");
	new Parameter<double>("d", &d,  1.576366639, this, "Equation parameter");
}

void BriereIITime::initialize()
{
    PhysiologicalTime::initialize();
    weather = seekOne<Model*>("weather");
}

double BriereIITime::calcDailyTimeStep()
{
    double T = weather->pullValue<double>("Tavg");

	if (T <= b)
        step = 0;
    else if ( (T > b) && (T <= c) )
        step = a * T *(T - b) * pow((c - T),(1/d));
    else
        step = 0;	
    return step;
}

} //namespace

 

