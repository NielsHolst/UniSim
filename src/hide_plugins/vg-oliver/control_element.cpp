/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <cmath>
#include <usbase/test_num.h>
#include <usbase/utilities.h>
#include "control_element.h"
#include "general.h"
#include "publish.h"

using namespace UniSim;

namespace vg {
	
PUBLISH(ControlElement)

/*! \class ControlElement
 * \brief Control elements which let _state_ approach _signal_ at a fixed rate
 *
 * Inputs
 * ------
 * - _rate_ is the rate at which _state_ approaches _signal_ [min<SUP>-1</SUP>]
 *
 * Outputs
 * ------
 * - see ControlElementBase

 * Default dependencies
 * ------------
 * - see ControlElementBase
 */

ControlElement::ControlElement(Identifier name, QObject *parent)
    : ControlElementBase(name, parent)
{
    Input(double, rate, 0.1);
}

double ControlElement::change(double error) {
    if (error == 0.) return 0.;
    double change = fabs(error)*(1. - negExp(rate/60*timeStepSecs));
    return change*sign(error);
}

} //namespace
