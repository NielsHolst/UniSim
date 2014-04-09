/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "energy_screen_controller.h"

using std::min;
using namespace UniSim;

namespace vg {
	
EnergyScreenController::EnergyScreenController(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<double>(Name(radiationThreshold), 10., "Radiation threshold for using energy screen (W/m2)");
    addParameterRef<double>(Name(energyBalance), "./energyBalance[value]");
    addParameterRef<double>(Name(maxSignalAtHighHumidity), "../humidityBalance[signal]");
    addParameterRef<double>(Name(radiation), "outdoors[radiation]");
    addVariable<double>(Name(signal),"Signal to screen [0;1]");
}

void EnergyScreenController::reset() {
    signal = 0;
}

void EnergyScreenController::update() {
    bool lowRadiation = (radiation < radiationThreshold);
    bool negativeEnergyBalance = (energyBalance <= 0.);
    signal = (lowRadiation && negativeEnergyBalance) ? min(1.,maxSignalAtHighHumidity) : 0.;
}

} //namespace
