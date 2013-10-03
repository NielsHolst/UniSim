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

using namespace UniSim;

namespace vg {
	
ControlElementAsym::ControlElementAsym(Identifier name, QObject *parent)
    : ControlElementBase(name, parent)
{
    addParameter<double>(Name(rateUp), 0.1, "Rate at which state approaches signal (per minute), when state is going @B up towards signal");
    addParameter<double>(Name(rateDown), 0.01, "Rate at which state approaches signal (per minute), when state is going @B down towards signal");
}

double ControlElementAsym::change(double error) {
    if (error == 0.) return 0.;
    double rate = (error > 0.) ? rateUp : rateDown;
    double change = fabs(error)*(1. - negExp(rate*60*timeStepSecs/fabs(error)));
    return change*sign(error);
}

} //namespace
