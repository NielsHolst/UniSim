/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "energy_screen_balance.h"

using namespace UniSim;

namespace vg {
	
EnergyScreenBalance::EnergyScreenBalance(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(indoorsDirectRadiation), "indoors/radiation[total]");
    addParameter<double>(Name(indoorsTemperature), 20., "Change to indoors temperature (oC)");
    addParameterRef<double>(Name(outdoorsTemperature), "outdoors[temperature]");
//    addParameterRef<double>(Name(KCover), "construction[KCover]");
    addVariable<double>(Name(value),"Value of energy balance (W/m2)");
}

void EnergyScreenBalance::reset() {
    value = 0;
}

void EnergyScreenBalance::update() {
    value = indoorsDirectRadiation - KCover*(indoorsTemperature - outdoorsTemperature);
}

} //namespace
