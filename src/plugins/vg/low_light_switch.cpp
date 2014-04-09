/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "low_light_switch.h"

using namespace UniSim;

namespace vg {
	
LowLightSwitch::LowLightSwitch(Identifier name, QObject *parent)
    : TimeSwitch(name, parent)
{
    addParameter<double>(Name(threshold), 20., "Outdoors radiation below which to switch on (W/m2)");
    addParameterRef<double>(Name(outdoorsRadiation), "outdoors[radiation]");
    addVariable<>(Name(lightOn), "Switch on according to threshold?");

//    Input(threshold).reset(40);
//    Input(outdoorsRadiation).from("outdoors[radiation]");
//    Output(on).reset(false).text("Switch on according to threshold?");
}

void LowLightSwitch::reset() {
    lightOn = false;
}

void LowLightSwitch::update() {
    lightOn = outdoorsRadiation < threshold;
}


} //namespace

