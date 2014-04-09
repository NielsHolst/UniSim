/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SWITCH_SETPOINT_H
#define VG_SWITCH_SETPOINT_H

#include <QTime>
#include <usbase/model.h>

namespace vg {

class SwitchSetpoint : public UniSim::Model
{
public:
    SwitchSetpoint(UniSim::Identifier name, QObject *parent);

private:
    // Parameter
    bool on;

    // Variables
    double setpoint;

};
} //namespace


#endif
