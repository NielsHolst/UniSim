/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include "general.h"
#include "proportional_control.h"
#include "publish.h"

using namespace UniSim;

namespace vg {
	
PUBLISH(ProportionalControl)

/*! \class ProportionalControl
 * \brief Provides proportional control towards a target value
 *
 * Proportional control provides a _signal_ to control the _actualValue_ towards the _targetValue_.
 * The size of the _signal_ depends on the size of the gap between the actual and the target value.
 * When the gap is in the range of the proportional bad [0;_pBand_], the signal is proportional to the gap.
 *
 * For further details, see DirectedControl
 *
 * Inputs
 * ------
 * - see DirectedControl
 *
 * Outputs
 * -------
 * - see DirectedControl
 *
 * Default dependencies
 * ------------
 * - none
 */

ProportionalControl::ProportionalControl(Identifier name, QObject *parent)
    : DirectedControl(name, parent)
{
}

void ProportionalControl::update() {
    double gap = (_direction == Ceiling) ?
                actualValue - targetValue :
                targetValue - actualValue;
    signal = (gap <= 0.) ? minSignal
             :
             (gap >= pBand) ? maxSignal
             :
             minSignal + gap/pBand*(maxSignal - minSignal);
}


} //namespace

