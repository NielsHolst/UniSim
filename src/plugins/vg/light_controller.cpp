/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QList>
#include <QMessageBox>
#include "light_controller.h"
#include "publish.h"

using namespace UniSim;

namespace vg {
	
PUBLISH(LightController)

/*! \class LightController
 * \brief Flags whether lights are on or off
 *
 * Inputs
 * ------
 * - _lightOff_ flags are read from all TimeSwitch child models with this port [true,false]
 * - _lightOn_ flags are read from all TimeSwitch child models with this port [true,false]
 * - _timeOn_ flags are read from all TimeSwitch child models with this port [true,false]
 *
 * Outputs
 * ------
 * - _signal_ is the on/off signal to lamps [true,false]
 *
 * Default dependencies
 * ------------
 * - zero or more children models of type TimeSwitch with one of these three ports:
 *   + _lightOff_ [true,false]
 *   + _lightOn_ [true,false]
 *   + _timeOn_ [true,false]
 *
 * To set _signal_ to lamps on or off, the light controller
 */

LightController::LightController(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Output(bool, signal);
}

void LightController::amend() {
    periodFlag = getFlag("periods");
    onFlag = getFlag("on");
    offFlag = getFlag("off");
}

const bool * LightController::getFlag(QString name) {
    auto model = seekOneChild<Model*>(name);
    return model->pullValuePtr<bool>("flag");
}

void LightController::reset() {
    update();
}

void LightController::update() {
    if (!*periodFlag)
        signal = false;
    else if (signal == 1. && *offFlag)
        signal = false;
    else if (signal == 0. && *onFlag)
        signal = true;
}

} //namespace

