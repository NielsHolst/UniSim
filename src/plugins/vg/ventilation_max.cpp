/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "ventilation_max.h"

using namespace UniSim;

namespace vg {
	
VentilationMax::VentilationMax(Identifier name, QObject *parent)
	: Model(name, parent)
{

    addParameterRef<double>(Name(radiation), "environment[radiation]");
    addParameter<double>(Name(thresholdRadiation), 10., "Threshold radiation day vs. night (W/m2)");
    addParameter<double>(Name(dayMax), 100., "Max. ventilation opening in the day [0;100]");
    addParameter<double>(Name(nightMax), 5., "Max. ventilation opening in the night [0;100]");
    addVariable<double>(Name(signal), "Max. ventilation opening  [0;100]");
}

void VentilationMax::reset() {
    signal = 0.;
}

void VentilationMax::update() {
    signal = (radiation < thresholdRadiation) ? nightMax : dayMax;
}


} //namespace

