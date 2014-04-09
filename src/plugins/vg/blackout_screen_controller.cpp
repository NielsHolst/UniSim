/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "energy_screen_controller.h"
#include "blackout_screen_controller.h"

using std::min;
using namespace UniSim;

namespace vg {
	
BlackoutScreenController::BlackoutScreenController(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<QTime>(Name(fromTime), QTime(18,0), "Screen is fully drawn after this time");
    addParameter<QTime>(Name(toTime), QTime(6,0), "Screen is fully drawn until this time");
    addParameter<double>(Name(radiationThreshold), 0., "Radiation threshold for using blackout screen (W/m2)");
    addParameter<bool>(Name(followEnergyScreen), true, "If the energy screen signal is larger than this, will the blackout screen use that signal instead?");
    addParameterRef<double>(Name(maxSignalAtHighHumidity), "../humidityBalance[signal]");
    addParameterRef<QTime>(Name(time), "calendar[timeOfDay]");
    addParameterRef<double>(Name(radiation), "outdoors[radiation]");
    addVariable<double>(Name(signal),"Signal to shade screen [0;1]");
}

void BlackoutScreenController::initialize() {
    EnergyScreenController *energyScreenController = peekOneSibling<EnergyScreenController*>("*");
    energyScreenSignal = energyScreenController ?
                energyScreenController->pullValuePtr<double>("signal") : 0;
}

void BlackoutScreenController::reset() {
    signal = 0;
}

void BlackoutScreenController::update() {
    bool lightOut = (radiation > radiationThreshold);
    bool darknessRequired = (time > fromTime || time < toTime);
    signal = (lightOut && darknessRequired) ? min(1.,maxSignalAtHighHumidity) : 0.;
    // Energy control
    bool saveEnergy = (followEnergyScreen && energyScreenSignal && *energyScreenSignal > signal);
    if (saveEnergy)
        signal = *energyScreenSignal;
}

} //namespace
