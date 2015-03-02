/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <cmath>
#include <usbase/test_num.h>
#include <usbase/utilities.h>
#include "linear_control_element.h"
#include "general.h"
#include "publish.h"

using namespace std;
using namespace UniSim;

namespace vg {
	
PUBLISH(LinearControlElement)

/*! \class LinearControlElement
 * \brief Controls _state_ linearly towards _signal_
 *
 * Inputs
 * ------
 * - _rate_ is the fixed rate at which _state_ approaches _signal_ [(as _state_ and _signal_)  min<SUP>-1</SUP>]
 *
 * Outputs
 * ------
 * - see ControlElementBase

 * Default dependencies
 * ------------
 * - see ControlElementBase
 */

LinearControlElement::LinearControlElement(Identifier name, QObject *parent)
    : BaseControlElement(name, parent)
{
    Input(double, rate, 1.);
}

double LinearControlElement::change(double error) {
    double step = rate/60*timeStepSecs;
    return sign(error)*min(step, abs(error));
}

} //namespace
