/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "temperature_setpoints.h"

using namespace UniSim;

namespace vg {
	
TemperatureSetpoints::TemperatureSetpoints(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(humidityIncrement), "./humidityIncrement[signal]");
    addParameterRef<double>(Name(humidityDecrement), "./humidityDecrement[signal]");
    addParameter<double>(Name(setMinimum), 20., "Desired minimum temperature, which may be increased by humidity (oC)");
    addParameter<double>(Name(setMaximum), 27., "Desired maximum temperature, which may be lowered by humidity (oC)");
    addVariable<double>(Name(minimum), "Desired minimum temperature, corrected for humidity (oC)");
    addVariable<double>(Name(maximum), "Desired maximum temperature, corrected for humidity (oC)");
}

void TemperatureSetpoints::update() {
    minimum = setMinimum + humidityIncrement;
    maximum = setMaximum - humidityDecrement;
}


} //namespace

