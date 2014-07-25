/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include "adjustable_setpoint.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(AdjustableSetpoint)

/*! \class AdjustableSetpoint
 * \brief An adjustable setpoint offers a basic setpoint that can be incremented or decremented
 *
 * Inputs
 * ------
 * - _baseSetpoint_ is the setpoint that may be adjusted by _adjustment_
 * - _adjustment_ will be added to or subtracted from _baseSetpoint_ according to _adjustmentDirection_
 * - _direction_ sets the direction of _adjustment_, either "increment" or "decrement"
 *
 * Output
 * ------
 * - _setpoint_ is the current value

 * Default dependencies
 * ------------
 * - none
 */

AdjustableSetpoint::AdjustableSetpoint(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, baseSetpoint, 0);
    Input(double, adjustment, 0);
    Input(QString, direction, "increment");
    Output(double, setpoint);
}

/*! \brief Parses _adjustmentDirection_
 *
 */
void AdjustableSetpoint::initialize() {
    QString s{direction.toLower()};
    if (s == "increment")
        sign = 1;
    else if (s == "decrement")
        sign = -1;
    else {
        QString msg{"Illegal 'adjustmentDirection'. Must be 'increment' or 'decrement' not '%1'"};
        throw Exception(msg.arg(direction), this);
    }
}

void AdjustableSetpoint::update() {
    setpoint = baseSetpoint + sign*adjustment;
}


} //namespace

