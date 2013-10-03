/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "light_controller.h"

using namespace UniSim;

namespace vg {
	
LightController::LightController(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<int>(Name(onDay), 300, "Julian day when to switch on (1..365)");
    addParameter<int>(Name(offDay), 60, "Julian day when to switch off (1..365)");
    addParameter<QTime>(Name(onTime), QTime(8,0), "Time when to switch on");
    addParameter<QTime>(Name(offTime), QTime(24,0), "Time when to switch off");
    addParameter<double>(Name(onRadiation), 0., "Outdoors radiation when to switch on (W/m2)");
    addParameter<double>(Name(offRadiation), 20., "Outdoors radiation when to switch on (W/m2)");

    addParameterRef<int>(Name(day), "calendar[dayOfyear] ");
    addParameterRef<QTime>(Name(time), "calendar[timeOfDay]");
    addParameterRef<double>(Name(radiation), "environment[radiation]");

    addVariable<double>(Name(signal), "Signal to lamp (0 or 1)");
    addVariable<bool>(Name(signalDay), "Switch on according to day?");
    addVariable<bool>(Name(signalTime), "Switch on according to time of day?");
    addVariable<bool>(Name(signalRadiation), "Switch on according to outdoors radiation?");
}

void LightController::reset() {
    signal = 0.;
}

void LightController::update() {
    signalDay = (onDay < offDay) ?
                 (day >= onDay) && (day <= offDay) :
                 (day >= onDay) || (day <= offDay);
    signalTime = (onTime < offTime) ?
                  (time >= onTime) && (time <= offTime) :
                  (time >= onTime) || (time <= offTime);
    signalRadiation = (radiation >= onRadiation) && (radiation <= offRadiation);
    signal = (signalDay && signalTime && signalRadiation) ? 1. : 0.;
}


} //namespace

