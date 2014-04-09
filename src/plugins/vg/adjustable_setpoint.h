/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_ADJUSTABLE_SETPOINT_H
#define VG_ADJUSTABLE_SETPOINT_H

#include <usbase/model.h>

namespace vg {

class AdjustableSetpoint : public UniSim::Model
{
public:
    AdjustableSetpoint(UniSim::Identifier name, QObject *parent);
    void initialize();
    void update();
private:
    // Parameters
    double baseSetpoint, adjustment;
    QString adjustmentDirection;

    // Variables
    double setpoint;

    // Data
    int sign;
};
} //namespace


#endif
