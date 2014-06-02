/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "publish.h"
#include "switch_setpoint.h"

using namespace UniSim;

namespace vg {
	
PUBLISH(SwitchSetpoint)

/*! \class SwitchSetpoint
 * \brief A setpoint with a on/off switch
 *
 * Inputs
 * ------
 * - _on_ is a switch to select either the on or of setpoint [true, false]
 * - _onSetpoint_ is the setpoint when on [R]
 * - _offSetpoint_ is the setpoint when off [R]
 * - _initSetpoint_ is the initial setpoint [R]
 *
 * Outputs
 * ------
 * - _setpoint_ is the setpoint value [R]

 * Default dependencies
 * ------------
 * - one child model with a _timeOn_ port [true,false]
 */

SwitchSetpoint::SwitchSetpoint(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, onSetpoint, 1.);
    Input(double, offSetpoint, 0.);
    Input(double, initSetpoint, 0.);
    InputRef(bool, on, "./*[timeOn]");
    Output(double, setpoint);
}

void SwitchSetpoint::reset() {
    setpoint = initSetpoint;
}

void SwitchSetpoint::update() {
    setpoint = on ? onSetpoint : offSetpoint;
}

} //namespace

