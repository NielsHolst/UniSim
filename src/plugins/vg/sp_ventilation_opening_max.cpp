/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "sp_ventilation_opening_max.h"

using namespace UniSim;

namespace vg {
	
SpVentilationOpeningMax::SpVentilationOpeningMax(Identifier name, QObject *parent)
	: Model(name, parent)
{

    addParameterRef<double>(Name(globRad), "outdoors/records[globRad]");
    addParameter<double>(Name(thresholdRad), 10., "Threshold radiation day vs. night (W/m2)");
    addParameter<double>(Name(day), 5., "Max. ventilation opening in the day [0;100]");
    addParameter<double>(Name(night), 5., "Max. ventilation opening in the night [0;100]");
    addVariable<double>(Name(value), "Max. ventilation opening  [0;100]");
}

void SpVentilationOpeningMax::reset() {
    update();
}

void SpVentilationOpeningMax::update() {
    value = (globRad < thresholdRad) ? night : day;
}


} //namespace

