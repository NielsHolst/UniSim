/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include "adjustable_setpoint.h"
#include "controlled_switch_setpoint.h"
#include "switch_setpoint.h"
#include "time_switch.h"

using namespace UniSim;

namespace vg {
	
/*! \class AdjustableSetpoint
 * \brief An adjustable setpoint offers a basic setpoint that can be incremented or decremented
 * - \em `double` baseSetpoint is the setpoint that may be adjusted by \em adjustment
 * - \em `double` adjustment will be added to or subtracted from \em baseSetpoint according to \em adjustmentDirection
 * - \em `double` adjustmentDirection sets the direction of \em adjustment, either "increment" or "decrement"
 */

/*!
 * \brief Constructs an adjustable setpoint model with \em name and \em parent
 */
AdjustableSetpoint::AdjustableSetpoint(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<double>(Name(baseSetpoint), 0., "Base setpoint to be adjusted'");
    addParameter<double>(Name(adjustment), 0., "Adjustment to base setpoint'");
    addParameter<QString>(Name(adjustmentDirection), QString(), "Direction of adjustment, either 'increment' or 'decrement'");
    addVariable<double>(Name(setpoint), "Calculated setpoint according to base setpoint and adjustment");
}

/*!
 * \brief Parses \em adjustmentDirection
 */
void AdjustableSetpoint::initialize() {
    QString s{adjustmentDirection.toLower()};
    if (s == "increment")
        sign = 1;
    else if (s == "decrement")
        sign = -1;
    else {
        QString msg{"Illegal 'adjustmentDirection'. Must be 'increment' or 'decrement' not '%1'"};
        throw Exception(msg.arg(adjustmentDirection), this);
    }
}

/*!
 * \brief Updates \em setpoint according to parameter values
 */
void AdjustableSetpoint::update() {
    setpoint = baseSetpoint + sign*adjustment;
}


} //namespace

