/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "greenhouse_energy.h"

using namespace UniSim;

namespace vg {
	
GreenhouseEnergy::GreenhouseEnergy(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("spScreenEnergy", &spScreenEnergy, 0., this,"Set point for energy screen [0;1]");
    new Parameter<double>("KScreenEnergy", &KScreenEnergy, 5.2, this, "K-value for energy screen (W/m2/K)");
    new Parameter<double>("KCover", &KCover, 7.9, this, "K-value for greenhouse cover (W/m2/K)");
    new Parameter<double>("bnLight", &bnLight, 0., this, "Longwave radiation exchanged from the canopy (W/m2)");
    new Parameter<double>("rnLight", &rnLight, 0., this, "Shortwave radiation reaching the canopy (W/m2)");
    new Parameter<double>("Toutdoors", &Toutdoors, 20., this, "Outdoors temperature (oC)");
    new Parameter<double>("Tindoors", &Tindoors, 25., this, "Indoors temperature (oC)");
    new Parameter<double>("ventilation", &ventilation, 0., this, "Greenhouse ventilation rate (m3 air/m2 greenhouse/s");
    new Parameter<double>("stdVentilation", &stdVentilation, 0., this, "Greenhouse ventilation rate normalised to a wind speed of 4 m/s (m3 air/m2 greenhouse/s");
    new Parameter<double>("floorHec", &floorHec, 6., this, "Heat capacity of the floor (W/m2/K)");
    new Parameter<double>("LEtot", &LEtot, 0., this, "Crop latent heat flux Weihong/Goudriaan (W/m2");
    new Parameter<double>("dewCondensation", &dewCondensation, 0., this, "Dew condensation rate (kg/m2/s");
    new Parameter<double>("lampHeat", &lampHeat, 0., this, "Convective heat from lamps (W/m2)");
    new Parameter<double>("spHeating", &spHeating, 0., this, "Setpoint for heating (oC)");
    new Variable<double>("heatingDemand", &heatingDemand, this, "Energy demand for heating (W/m2)");
    new Variable<double>("Tunheated", &Tunheated, this, "Greenhouse temperature if not heated (oC)");
    new Variable<double>("energyBalance", &energyBalance, this, "Energy balance of greenhouse (W/m2)");
    new Variable<double>("heatCapacity", &heatCapacity, this, "Heat capacity of greenhouse (W/m2/K)");
}

void GreenhouseEnergy::reset() {
    update();
}

void GreenhouseEnergy::update() {
    double Kcov = (1-spScreenEnergy)*KCover + spScreenEnergy*KScreenEnergy;
    double ventTot = ventilation + stdVentilation;
    energyBalance =
        (Kcov + ventTot*CpAir)*(Toutdoors+T0) - LEtot + LHe*dewCondensation
        + lampHeat + rnLight - bnLight + floorHec*(Tindoors+T0);
    heatCapacity  = Kcov + ventTot*CpAir + floorHec;
    double energyDemand = heatCapacity*(spHeating+T0);
    heatingDemand = energyDemand - energyBalance;
    Tunheated = energyBalance/heatCapacity - T0;
}


} //namespace

