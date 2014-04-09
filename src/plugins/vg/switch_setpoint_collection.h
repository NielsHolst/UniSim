/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SWITCH_SETPOINT_COLLECTION_H
#define VG_SWITCH_SETPOINT_COLLECTION_H

#include <usbase/model.h>

namespace vg {

class SwitchSetpointCollection : public UniSim::Model
{
public:
    SwitchSetpointCollection(UniSim::Identifier name, QObject *parent);
    void amend();
    void reset();
    void update();
private:
    // Parameter value
    double emptyValue;
    bool emptyAllowed;
    // Variables
    double setpoint;

    // Data
    struct SwitchValue {
        const double *value;
        const bool *on;
    };
    QList<SwitchValue> setpoints;
};
} //namespace


#endif
