/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "heating_flow_controller.h"

using namespace UniSim;

namespace vg {
	
HeatingFlowController::HeatingFlowController(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(indoorsTemperature), "indoors/temperature[value]");
    addParameterRef<double>(Name(heatingSetpoint), "setpoints/temperature/heating[setpoint]");
    addVariable<double>(Name(signal), "Water flow on (1) or off (0)");
}

void HeatingFlowController::reset() {
    signal = 1.;
}

void HeatingFlowController::update() {
    signal = indoorsTemperature < heatingSetpoint ? 1. : 0.;
}



} //namespace

