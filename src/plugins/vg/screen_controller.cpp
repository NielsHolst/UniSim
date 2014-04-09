/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "screen_controller.h"

using namespace UniSim;

namespace vg {
	
ScreenController::ScreenController(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<double>(Name(maxSignalAtHighHumidity), 0.95, "Maximum signal to screen at high humidity [0;1]");
}

void ScreenController::amend() {

}

void ScreenController::update() {
    // Sort out interactions
}

} //namespace
