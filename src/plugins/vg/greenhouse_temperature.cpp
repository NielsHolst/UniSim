/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "greenhouse_temperature.h"
#include "pipe.h"

using namespace UniSim;

namespace vg {
	
GreenhouseTemperature::GreenhouseTemperature(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(energyBalance), "greenhouse/temperature/energy[energyBalance]");
    addParameterRef<double>(Name(heatCapacity), "greenhouse/temperature/energy[heatCapacity]");
    addParameterRef<double>(Name(pipesHeatTransfer), "greenhouse/temperature/pipes[heatTransfer]");
    addVariable<double>(Name(temperature), "Current greenhouse temperature (oC)");
}

void GreenhouseTemperature::reset() {
    update();
}

void GreenhouseTemperature::update() {
    temperature = (energyBalance + pipesHeatTransfer)/heatCapacity;
}




} //namespace

