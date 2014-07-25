/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "ventilation_latent_energy_balance.h"

using namespace UniSim;

namespace vg {
	
VentilationLatentEnergyBalance::VentilationLatentEnergyBalance(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(timeStepSecs), "calendar[timeStepSecs]");
    addParameterRef<double>(Name(groundArea), "greenhouse/construction[groundArea]");
    addParameterRef<double>(Name(height), "greenhouse/construction[averageHeight]");
    addParameterRef<double>(Name(ventilation), "greenhouse/ventilation[value]");
    addParameterRef<double>(Name(mcIndoors), "climate/humidity[mc]");
    addParameterRef<double>(Name(mcOutdoors), "environment[mc]");
    addVariable<double>(Name(value), "Balance of latent heat by ventilation (W/m2)");
    addVariable<double>(Name(totalLoss), "Total latent heat lost through ventilation (MJ/m2)");
}

void VentilationLatentEnergyBalance::reset() {
    value = totalLoss = 0.;
}

void VentilationLatentEnergyBalance::update() {
    value = height*ventilation*(mcOutdoors - mcIndoors)*LHe*RhoAir/groundArea;
    totalLoss += -value*timeStepSecs/1e6;
}

} //namespace

