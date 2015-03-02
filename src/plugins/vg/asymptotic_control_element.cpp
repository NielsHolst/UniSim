/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <cmath>
#include <usbase/test_num.h>
#include <usbase/utilities.h>
#include "asymptotic_control_element.h"
#include "general.h"
#include "publish.h"

using namespace UniSim;

namespace vg {
	
PUBLISH(AsymptoticControlElement)

/*! \class AsymptoticControlElement
 * \brief Controls _state_ asymptotically towards _signal_
 *
 * Inputs
 * ------
 * - _rate_ is the relative rate at which _state_ approaches _signal_ [min<SUP>-1</SUP>]
 *
 * Outputs
 * ------
 * - see ControlElementBase

 * Default dependencies
 * ------------
 * - see ControlElementBase
 */

AsymptoticControlElement::AsymptoticControlElement(Identifier name, QObject *parent)
    : BaseControlElement(name, parent)
{
    Input(double, rate, 0.1);
}

double AsymptoticControlElement::change(double error) {
    if (error == 0.) return 0.;
    double change = fabs(error)*(1. - negExp(rate/60*timeStepSecs));
    return change*sign(error);
}

} //namespace
