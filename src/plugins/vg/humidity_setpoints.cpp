/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "humidity_setpoints.h"

using namespace UniSim;

namespace vg {
	
HumiditySetpoints::HumiditySetpoints(Identifier name, QObject *parent)
    : Model(name, parent)
{
    addParameterRef<double>(Name(radiation), "environment[radiation]");
    addParameter<double>(Name(daylightThreshold), 10., "Global radiation threshold for daylight (W/m2)");
    addParameter<double>(Name(maxRHDay), 80., "Setpoint for relative humidity during the day (%)");
    addParameter<double>(Name(maxRHNight), 90., "Setpoint for relative humidity during the night (%)");
    addParameter<double>(Name(minDeltaXBasis), 1., "Setpoint for delta x (g/m3)");
    addVariable<double>(Name(maxRH), "Setpoint for maximum relative humidity (%)");
    addVariable<double>(Name(minDeltaX), "Setpoint for minimum delta x (g/m3)");
}

void HumiditySetpoints::reset() {
    update();
}

void HumiditySetpoints::update() {
    maxRH = (radiation < daylightThreshold) ? maxRHNight : maxRHDay;
    minDeltaX = minDeltaXBasis;
}


} //namespace

