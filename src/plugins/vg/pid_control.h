/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_PID_CONTROL_H
#define VG_PID_CONTROL_H

#include "directed_control.h"

namespace vg {

class PidControl : public DirectedControl
{
public:
    PidControl(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    // Parameters
    double actualValue, targetValue, initSignal, Kp, Ti, Td, timeStepSecs;

    // Variables
    double signal;

    // Data
    double integral, prevValue;
};
} //namespace


#endif
