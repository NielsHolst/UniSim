/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QList>
#include <QMessageBox>
#include "light_controller.h"
#include "time_switch.h"
#include "high_light_switch.h"
#include "low_light_switch.h"
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
 */

LightController::LightController(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addVariable<bool>(Name(signal), "Signal to lamp (0 or 1)");
}

void LightController::amend() {
    auto timeSwitches = seekChildren<TimeSwitch*>("*");
    for (TimeSwitch *sw : timeSwitches) {
        if (dynamic_cast<HighLightSwitch*>(sw))
            highLightSwitchesOff << sw->pullValuePtr<bool>("lightOff");
        else if (dynamic_cast<LowLightSwitch*>(sw))
            lowLightSwitchesOn << sw->pullValuePtr<bool>("lightOn");
        else
            timeSwitchesOn << sw->pullValuePtr<bool>("timeOn");
    }
}

void LightController::reset() {
    signal = false;
}

void LightController::update() {
    if (!onPeriod()) {
        signal = false;
    }
    else if (signal == 1.) {
        bool off = true;
        for (auto switchOff : highLightSwitchesOff)
            off = off || *switchOff;
        signal = !off;
    }
    else {
        bool on = true;
        for (auto switchOn : lowLightSwitchesOn)
            on = on || *switchOn;
        signal = on;
    }
}

bool LightController::onPeriod() {
    bool on = true;
    for (auto switchOn : timeSwitchesOn)
        on = on || *switchOn;
    return on;
}



} //namespace

