/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "high_light_switch.h"

using namespace UniSim;

namespace vg {
	
HighLightSwitch::HighLightSwitch(Identifier name, QObject *parent)
    : TimeSwitch(name, parent)
{
    addParameter<double>(Name(threshold), 40., "Outdoors radiation above which to switch off (W/m2)");
    addParameterRef<double>(Name(outdoorsRadiation), "outdoors[radiation]");
    addVariable<bool>(Name(lightOff), "Switch off according to threshold?");
}

void HighLightSwitch::reset() {
    lightOff = false;
}

void HighLightSwitch::update() {
    lightOff = outdoorsRadiation > threshold;
}


} //namespace

