/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "general.h"
#include "sp_ventilation_opening.h"
#include <usbase/utilities.h>

using namespace std;
using namespace UniSim;

namespace vg {
	
SpVentilationOpening::SpVentilationOpening(Identifier name, QObject *parent)
	: Model(name, parent)
{

    new Parameter<double>("alphaTemperature", &alphaTemperature, 0., this, "Alpha determined by temperature [0;100]");
    new Parameter<double>("alphaHumidity", &alphaHumidity, 0., this, "Alpha determined by humidity [0;100]");
    new Parameter<double>("alphaMax", &alphaMax, 100., this, "Max opening allowed [0;100]");
    new Parameter<double>("Tindoors", &Tindoors, 25., this, "Indoors temperature (oC))");
    new Parameter<double>("Toutdoors", &Toutdoors, 20., this, "Outdoors temperature (oC))");
    new Parameter<double>("spTemperature", &spTemperature, 23., this, "Setpoint temperature for ventilation opening (oC))");
    new Parameter<double>("windspeed", &windspeed, 0., this, "Setpoint temperature for ventilation opening (oC))");
    new Variable<double>("alphaLeeSide", &alphaLeeSide, this, "Ventilation opening on the lee side [0;100]");
    new Variable<double>("alphaWindSide", &alphaWindSide, this, "Ventilation opening on the wind side [0;100]");
}

void SpVentilationOpening::reset() {
    alphaLeeSide = alphaWindSide = 0;
}

void SpVentilationOpening::update() {
    alphaLeeSide = max(correctedAlphaHumidity(), correctedAlphaTemperature());
    alphaWindSide = 0.75*alphaLeeSide;
}

double SpVentilationOpening::correctedAlphaHumidity() {
    bool tooColdIndoors = (Tindoors < spTemperature - 15);
    bool tooColdOutdoors = Toutdoors < 4;
    if (tooColdIndoors || tooColdOutdoors)
        return 0.;

    double Tdiff = Tindoors - Toutdoors;
    double alphaTdiff = alphaMax - 1./(0.75*exp(-Tdiff/10));
    if (alphaTdiff < 0)
        alphaTdiff = 0;
    return min(alphaHumidity, alphaTdiff);
}

double SpVentilationOpening::correctedAlphaTemperature() {
    double alpha = 100*alphaTemperature/(max(windspeed,1.)/2);
    if (alpha < 0)
        alpha = 0;
    else if (alpha > 100)
        alpha = 100;
    return alpha;
}


} //namespace

