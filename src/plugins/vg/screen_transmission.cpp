/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "screen_transmission.h"

using namespace UniSim;

namespace vg {
	
ScreenTransmission::ScreenTransmission(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(screenState), "../control[state]");
    addParameter<double>(Name(ratio), 0.3, "Ratio of transmission [0;1]");
    addVariable<double>(Name(value), "Screen transmission [0;1]");
}

void ScreenTransmission::reset() {
    value = 1.;
}

void ScreenTransmission::update() {
    value = 1. - screenState + ratio*screenState;
}

} //namespace

