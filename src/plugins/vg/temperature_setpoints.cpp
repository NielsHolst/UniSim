/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "switch_setpoint.h"
#include "temperature_setpoints.h"
#include "time_switch.h"

using namespace UniSim;

namespace vg {
	
TemperatureSetpoints::TemperatureSetpoints(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addVariable<double>(Name(baseTHeating), "Temperature for heating, which may be increased due to humidity (oC)");
    addVariable<double>(Name(baseTVentilation), "Temperature for ventilation, which may be lowered dur to humidity (oC)");
    addVariable<double>(Name(humidityIncrement), "Increment for baseTheating due to humidity (oC)");
    addVariable<double>(Name(humidityDecrement), "Decrement for baseTventilation due to humidity (oC)");
    addVariable<double>(Name(THeating), "Temperature for heating, corrected for humidity (oC)");
    addVariable<double>(Name(TVentilation), "Temperature for ventilation, corrected for humidity (oC)");
}

void TemperatureSetpoints::amend() {
    auto ventilationSetpoints = seekOneChild<Model*>("ventilationSetpoints")->seekChildren<SwitchSetpoint*>("*");
    for (auto sp : ventilationSetpoints) {
        TemperatureSwitch ts;
        ts.temperature = sp->pullValuePtr<double>("setpoint");
        ts.on = sp->pullValuePtr<bool>("on");
        ventilationTemperatureSwitches << ts;
    }
}

void TemperatureSetpoints::update() {
    THeating = baseTHeating + humidityIncrement;
    TVentilation = baseTVentilation - humidityDecrement;
}


} //namespace

