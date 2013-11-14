/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "energy_screen_controller.h"

using namespace UniSim;

namespace vg {
	
EnergyScreenController::EnergyScreenController(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<double>(Name(maxSignalHighHumidity), 0.95, "Maximum signal to screen at high humidity [0;1]");
    addParameter<double>(Name(radiationThreshold), 10., "Radiation threshold for using energy screen (W/m2)");
    addParameterRef<double>(Name(radiation), "environment[radiation]");
    addParameterRef<bool>(Name(isHumidityHigh), "climate/humidity[isHigh]");
    addVariable<double>(Name(signal),"Signal to screen [0;1]");
}

void EnergyScreenController::initialize() {
    Model *model = peekOneSibling<Model*>("energyBalance");
    energyBalance = model ? model->pullValuePtr<double>("value") : 0;
}

void EnergyScreenController::reset() {
    signal = 0;
}

void EnergyScreenController::update() {
    // Outdoors radiation control
    bool lowRadiation = (radiation < radiationThreshold);
    bool negativeEnergyBalance = energyBalance ? *energyBalance < 0. : true;
    signal = (lowRadiation && negativeEnergyBalance) ? 1. : 0.;
    // Humidity control
    if (isHumidityHigh && signal > maxSignalHighHumidity)
        signal = maxSignalHighHumidity;
}

} //namespace
