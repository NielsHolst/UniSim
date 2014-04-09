/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "co2_setpoints.h"

using namespace UniSim;

namespace vg {
	
Co2Setpoints::Co2Setpoints(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addVariable<double>(Name(signal), "Signal to lamp (0 or 1)");
}

//void Co2Controller::amend() {
//    auto timeSwitches = seekChildren<TimeSwitch*>("*");
//    for (TimeSwitch *sw : timeSwitches) {
//        if (dynamic_cast<HighLightSwitch*>(sw))
//            highLightSwitchesOff << sw->pullValuePtr<bool>("lightOff");
//        else if (dynamic_cast<LowLightSwitch*>(sw))
//            lowLightSwitchesOn << sw->pullValuePtr<bool>("lightOn");
//        else
//            timeSwitchesOn << sw->pullValuePtr<bool>("timeOn");
//    }
//}

//void Co2Controller::reset() {
//    signal = 0.;
//}

//void Co2Controller::update() {
//    if (!onPeriod()) {
//        signal = 0.;
//        return;
//    }
//    if (signal == 1.) {
//        bool off = true;
//        for (auto switchOff : highLightSwitchesOff)
//            off = off || *switchOff;
//        signal = off ? 0. : 1.;
//    }
//    else {
//        bool on = true;
//        for (auto switchOn : lowLightSwitchesOn)
//            on = on || *switchOn;
//        signal = on ? 1. : 0.;
//    }
//}

//bool Co2Controller::onPeriod() {
//    bool on = true;
//    for (auto switchOn : timeSwitchesOn)
//        on = on || *switchOn;
//    return on;
//}



} //namespace

