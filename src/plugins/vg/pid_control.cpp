/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include "general.h"
#include "pid_control.h"

using namespace UniSim;

namespace vg {
	
PidControl::PidControl(Identifier name, QObject *parent)
    : DirectedControl(name, parent)
{
    addParameterRef<double>(Name(timeStepSecs), "calendar[timeStepSecs]");
    addParameter<double>(Name(actualValue), 20., "Current value being regulated");
    addParameter<double>(Name(targetValue), 20., "Value to regulate towards");
    addParameter<double>(Name(initSignal), 0., "Initial value of signal");
    addParameter<double>(Name(Kp), 1., "Proportional gain");
    addParameter<double>(Name(Ti), 1e6, "Integral time");
    addParameter<double>(Name(Td), 0., "Derivative time");
    addVariable<double>(Name(signal), "Response of PID control");
}

void PidControl::reset() {
    signal = initSignal;
    integral = 0;
    prevValue = actualValue;
}

void PidControl::update() {
    double error = actualValue - targetValue;
    double slope = (actualValue - prevValue)/timeStepSecs;
    if (direction() == Ceiling) {
        error = -error;
    }
    signal = Kp*(error + integral/Ti + Td*slope);
}


} //namespace

