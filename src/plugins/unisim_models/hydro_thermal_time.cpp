/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "hydro_thermal_time.h"

namespace UniSim{

HydroThermalTime::HydroThermalTime(UniSim::Identifier name, QObject *parent)
    : DayDegrees(name, parent)
{
    new Parameter<double>("SWP0", &swp0, 0., this,
        "Soil water potential threshold (MPa)");
    new Parameter<double>("SWPTopt", &swpTopt, 100., this,
        "Temperature optimum ((@Char{ring}C). Above this value, the soil water potential threshold "
        "increases linearly by @F {Kt}");
    new Parameter<double>("Kt", &Kt, 0., this,
        "Rate of increase (MPa\"/\"(@Char{ring}C) in soil water potential threshold above @F {SWPTOpt}");

    new PullVariable<double>("swpThreshold", &swpThreshold, this,
        "Actual soil water potential threshold for germination (MPa)");
}

double HydroThermalTime::calcDailyTimeStep()
{    
    double T = weather->pullVariable<double>("Tavg");
    swpThreshold = T < swpTopt ? swp0 : Kt*(T - swpTopt);
    if (swpThreshold > 0.)
        swpThreshold = 0.;
		
    double swp = weather->pullVariable<double>("swp");
    if (swp < swpThreshold) return 0.;
	
    return DayDegrees::calcDailyTimeStep();
}

} //namespace

