/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "lamp_led.h"

using namespace UniSim;

namespace vg {
	
LampLed::LampLed(Identifier name, QObject *parent)
    : LampBase(name, parent)
{
    addParameter<double>(Name(density), 0.015, "Number of lamps per m2");
}

/*  Heat loss to the greenhouse, Wm-2, Thermal heat and long-wave
    radiation plus heat production of the transformator (10%) / assuming that 50% of that reaches the crop!
*/

void LampLed::calculate() {
    convectiveHeat = 0.02*capacity*density;
    bnLight = capacity*0.05;
    rnLight = capacity*0.82;
    par = capacity*0.82;
    energyUse = capacity;
}

} //namespace

