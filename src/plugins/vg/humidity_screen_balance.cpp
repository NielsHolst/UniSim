/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "humidity_screen_balance.h"


using namespace UniSim;

namespace vg {
	
HumidityScreenBalance::HumidityScreenBalance(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<double>(Name(signalAtHighHumidity), 0.95, "Maximum signal to screen at high humidity [0;1]");
    addParameterRef<bool>(Name(isHumidityHigh), "indoors/humidity[highHumidity]");
    addVariable<double>(Name(signal),"Signal to screens adjusted for humidity [0;1]");
}

void HumidityScreenBalance::reset() {
    signal = 1;
}

void HumidityScreenBalance::update() {
    signal = isHumidityHigh ? signalAtHighHumidity : 1.;
}

} //namespace
