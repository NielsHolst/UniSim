/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_PID_CONTROL_ELEMENT_H
#define VG_PID_CONTROL_ELEMENT_H

#include "base_control_element.h"

namespace vg {

class PidControlElement : public BaseControlElement
{
public:
    PidControlElement(UniSim::Identifier name, QObject *parent);
    void localReset();
    double change(double error);
private:
    // Inputs
    double Kprop, Kint, Kderiv;
    // Data
    double errorIntegral, errorDerivative, prevError;
    bool firstChange;
};
} //namespace


#endif
