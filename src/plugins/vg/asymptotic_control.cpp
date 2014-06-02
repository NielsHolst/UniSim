/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "asymptotic_control.h"
#include "publish.h"

using namespace UniSim;

namespace vg {
	
PUBLISH(AsymptoticControl)

/*! \class AsymptoticControl
 * \brief Provides asymptotic control towards a target value
 *
 * Asymptotic control provides a _signal_ to control the _actualValue_ towards the _targetValue_.
 * The size of the _signal_ depends on the size of the gap between the actual and the target value;
 * _signal is exponentially decreasing with this gap.
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

AsymptoticControl::AsymptoticControl(Identifier name, QObject *parent)
    : DirectedControl(name, parent)
{
}

void AsymptoticControl::update() {
    double gap = (_direction == Ceiling) ?
                actualValue - targetValue :
                targetValue - actualValue;
    signal = (gap <= 0.) ? minSignal
             :
             minSignal + (maxSignal - minSignal)*(1. - exp(-gap/pBand));
}


} //namespace

