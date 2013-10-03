/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "energy_screen_controller.h"
#include "shade_screen_controller.h"

using namespace UniSim;

namespace vg {
	
ShadeScreenController::ShadeScreenController(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<double>(Name(maxSignalHighHumidity), 0.95, "Maximum signal to screen at high humidity [0;1]");
    addParameter<double>(Name(radiationThreshold), 10., "Radiation threshold for using shade screen (W/m2)");
    addParameter<double>(Name(temperatureThreshold), 28., "Temperature threshold for using shade screen (oC)");
    addParameter<double>(Name(tolerance), 2000., "Low tolerance gives steep reaction and high tolerance gives gentle reaction, when threshold is crossed (>=0");
    addParameter<bool>(Name(followEnergyScreen), true, "If the energy screen signal is larger than this, will the blackout screen use that signal instead?");
    addParameterRef<double>(Name(radiation), "environment[radiation]");
    addParameterRef<double>(Name(temperature), "climate/temperature[value]");
    addParameterRef<bool>(Name(isHumidityHigh), "climate/humidity[isHigh]");
    addVariable<double>(Name(signal),"Signal to shade screen [0;1]");
}

void ShadeScreenController::initialize() {
    EnergyScreenController *energyScreenController = peekOneSibling<EnergyScreenController*>("*");
    energyScreenSignal = energyScreenController ?
                energyScreenController->pullValuePtr<double>("signal") : 0;
}

void ShadeScreenController::reset() {
    signal = 0.;
}

void ShadeScreenController::update() {
    // Temperature control
    double tempDiff = temperature - temperatureThreshold;
    signal = (tempDiff > 0. && radiation > radiationThreshold)  ?
        1 - exp(-tempDiff*radiation/tolerance) :
        0.;
    // Humidity control
    if (isHumidityHigh && signal > maxSignalHighHumidity)
        signal = maxSignalHighHumidity;
    // Interaction with energy screen
    else if (followEnergyScreen && energyScreenSignal && *energyScreenSignal > signal)
        signal = *energyScreenSignal;
}

} //namespace
