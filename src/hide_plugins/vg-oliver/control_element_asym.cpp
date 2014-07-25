/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <cmath>
#include <usbase/test_num.h>
#include <usbase/utilities.h>
#include "control_element_asym.h"
#include "general.h"
#include "publish.h"

using namespace UniSim;

namespace vg {
	
PUBLISH(ControlElementAsym)

/*! \class ControlElementAsym
 * \brief Control elements which let _state_ approach _signal_ asymmetrically, at different rates from above and below
 *
 * Inputs
 * ------
 * - _rateUp_ is the rate at which _state_ approaches _signal_ from below [min<SUP>-1</SUP>]
 * - _rateDown_ is the rate at which _state_ approaches _signal_ from above [min<SUP>-1</SUP>]
 *
 * Outputs
 * ------
 * - see ControlElementBase

 * Default dependencies
 * ------------
 * - see ControlElementBase
 */

ControlElementAsym::ControlElementAsym(Identifier name, QObject *parent)
    : ControlElementBase(name, parent)
{
    Input(double, rateUp, 0.1);
    Input(double, rateDown, 0.1);
}

double ControlElementAsym::change(double error) {
    if (error == 0.) return 0.;
    double rate = (error > 0.) ? rateUp : rateDown;
    double change = fabs(error)*(1. - negExp(rate*60*timeStepSecs/fabs(error)));
    return change*sign(error);
}

} //namespace
