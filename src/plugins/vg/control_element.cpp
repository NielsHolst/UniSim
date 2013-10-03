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

using namespace UniSim;

namespace vg {
	
ControlElement::ControlElement(Identifier name, QObject *parent)
    : ControlElementBase(name, parent)
{
    addParameter<double>(Name(rate), 0.1, "Rate at which state approaches signal (per minute)");
}

double ControlElement::change(double error) {
    if (error == 0.) return 0.;
    double change = fabs(error)*(1. - negExp(rate*60*timeStepSecs/fabs(error)));
    return change*sign(error);
}

} //namespace
