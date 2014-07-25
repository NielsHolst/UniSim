/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include "publish.h"
#include "switch_setpoint_collection.h"

using namespace UniSim;

namespace vg {

PUBLISH(SwitchSetpointCollection)

/*! \class SwitchSetpointCollection
 * \brief Picks a setpoint among child model setpoints
 *
 * Inputs
 * ------
 * - _defaultValue_ is the initial setpoint value, or if no child setpoints are on [R]
 *
 * Outputs
 * ------
 * - _setpoint_ is the value of the first child model setpoint that is on [R]

 * Default dependencies
 * ------------
 * - zero or more child models with two ports:
 *   + setpoint [R]
 *   + on [true,false]
 * - only child models with those two ports are allowed
 */

SwitchSetpointCollection::SwitchSetpointCollection(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, defaultValue, 0.);
    Output(double, setpoint);
}

void SwitchSetpointCollection::initialize() {
    setpoints.clear();
    auto setpointModels = seekChildren<Model*>("*");
    for (auto sp : setpointModels) {
        SwitchValue sv;
        sv.value = sp->pullValuePtr<double>("setpoint");
        sv.on = sp->pullValuePtr<bool>("on");
        setpoints << sv;
    }
}

void SwitchSetpointCollection::reset() {
    setpoint = defaultValue;
}

void SwitchSetpointCollection::update() {
    if (setpoints.isEmpty()) return;
    setpoint = defaultValue;
    for (auto sp : setpoints) {
        if (*sp.on) {
            setpoint = *sp.value;
            break;
        }
    }
}


} //namespace

