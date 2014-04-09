/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include "switch_setpoint.h"
#include "switch_setpoint_collection.h"

using namespace UniSim;

namespace vg {
	
SwitchSetpointCollection::SwitchSetpointCollection(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<double>(Name(emptyValue), 0., "Setpoint value if collection is empty");
    addParameter<bool>(Name(emptyAllowed), false, "Is an empty collection allowed?");
    addVariable<double>(Name(setpoint), "Calculated setpoint according to current time");
}

void SwitchSetpointCollection::amend() {
    auto setpointModels = seekChildren<Model*>("*");
    for (auto sp : setpointModels) {
        SwitchValue sv;
        sv.value = sp->pullValuePtr<double>("setpoint");
        sv.on = sp->pullValuePtr<bool>("on");
        setpoints << sv;
    }
    if (setpoints.isEmpty() && !emptyAllowed) {
        throw Exception("A child model of type 'SwitchSetpoint' is needed", this);
    }
}

void SwitchSetpointCollection::reset() {
    setpoint = emptyValue;
}

void SwitchSetpointCollection::update() {
    if (setpoints.isEmpty()) return;
    setpoint = *setpoints.first().value;
    for (auto sp : setpoints) {
        if (*sp.on) {
            setpoint = *sp.value;
            break;
        }
    }
}


} //namespace

