/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "energy_screen_controller.h"
#include "blackout_screen_controller.h"

using namespace UniSim;

namespace vg {
	
BlackoutScreenController::BlackoutScreenController(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<QTime>(Name(fromTime), QTime(18,0), "Screen is fully drawn after this time");
    addParameter<QTime>(Name(toTime), QTime(6,0), "Screen is fully drawn until this time");
    addParameter<double>(Name(maxSignalHighHumidity), 0.95, "Maximum signal to screen at high humidity [0;1]");
    addParameter<double>(Name(radiationThreshold), 0., "Radiation threshold for using blackout screen (W/m2)");
    addParameter<bool>(Name(followEnergyScreen), true, "If the energy screen signal is larger than this, will the blackout screen use that signal instead?");
    addParameterRef<QTime>(Name(time), "calendar[timeOfDay]");
    addParameterRef<double>(Name(radiation), "environment[radiation]");
    addParameterRef<bool>(Name(isHumidityHigh), "climate/humidity[isHigh]");
    addVariable<double>(Name(signal),"Signal to shade screen [0;1]");
}

void BlackoutScreenController::initialize() {
    EnergyScreenController *energyScreenController = peekOneSibling<EnergyScreenController*>("*");
    energyScreenSignal = energyScreenController ?
                energyScreenController->pullValuePtr<double>("signal") : 0;
}

void BlackoutScreenController::reset() {
    update();
}

void BlackoutScreenController::update() {
    // Time control
    signal = (time > fromTime || time < toTime) ? 1. : 0.;
    // Humidity control
    if (isHumidityHigh && signal > maxSignalHighHumidity)
        signal = maxSignalHighHumidity;
    // Interaction with energy screen
    else if (followEnergyScreen && energyScreenSignal && *energyScreenSignal > signal)
        signal = *energyScreenSignal;
}

} //namespace
