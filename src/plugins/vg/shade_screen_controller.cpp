/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "energy_screen_controller.h"
#include "shade_screen_controller.h"

using std::min;
using namespace UniSim;

namespace vg {
	
ShadeScreenController::ShadeScreenController(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(maxSignalAtHighHumidity), "../humidityBalance[signal]");
    addParameter<double>(Name(radiationThreshold), 10., "Radiation threshold for using shade screen (W/m2)");
    addParameter<double>(Name(tolerance), 2000., "Low tolerance gives steep reaction and high tolerance gives gentle reaction, when threshold is crossed (>=0");
    addParameter<bool>(Name(followEnergyScreen), true, "If the energy screen signal is larger than this, will the blackout screen use that signal instead?");
    addParameterRef<double>(Name(radiation), "outdoors[radiation]");
    addVariable<double>(Name(signal),"Signal to shade screen [0;1]");
}

void ShadeScreenController::initialize() {
    auto energyScreenController = peekOneSibling<EnergyScreenController*>("*");
    energyScreenSignal = energyScreenController ?
                energyScreenController->pullValuePtr<double>("signal") : 0;
}

void ShadeScreenController::reset() {
    signal = 0.;
}

void ShadeScreenController::update() {
    // Outdoors radiation control
    bool highRadiation = (radiation > radiationThreshold);
    signal = highRadiation ? min(1-exp(-radiation/tolerance), maxSignalAtHighHumidity)  : 0.;
    // Energy control
    bool saveEnergy = followEnergyScreen && energyScreenSignal && *energyScreenSignal > signal;
    if (saveEnergy)
        signal = *energyScreenSignal;
}

} //namespace
