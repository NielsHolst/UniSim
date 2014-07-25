/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "microclimate_temperature.h"

using namespace UniSim;

namespace vg {
	
MicroclimateTemperature::MicroclimateTemperature(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(energyBalance), "greenhouse/energetics[energyBalance]");
    addParameterRef<double>(Name(heatCapacity), "greenhouse/energetics[heatCapacity]");
    addParameterRef<double>(Name(pipesHeatFlux), "pipes[heatFlux]");
    addVariable<double>(Name(value),"Current microclimate temperature (oC)");
}


void MicroclimateTemperature::reset() {
    value = 20.;
}

void MicroclimateTemperature::update() {
    value = (energyBalance + pipesHeatFlux)/heatCapacity - T0;
}

} //namespace
