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
    new Parameter<double>("energyBalance", &energyBalance, 0., this, "Energy balance of greenhouse (W/m2)");
    new Parameter<double>("heatCapacity", &heatCapacity, 0., this, "Heat capacity of greenhouse (W/m2/K)");
    new Parameter<double>("pipesHeatTransfer", &pipesHeatTransfer, 0., this, "Heat transfer of all pipes (W/m2)");
    new Variable<double>("temperature", &temperature, this, "Current greenhouse temperature (oC)");
}

void GreenhouseTemperature::reset() {
    update();
}

void GreenhouseTemperature::update() {
    temperature = (energyBalance + pipesHeatTransfer)/heatCapacity;
}




} //namespace

