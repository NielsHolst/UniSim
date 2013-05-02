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
    addParameterRef<double>(Name(spScreenEnergy), "screen/energy[sp]");
    addParameterRef<double>(Name(bnLight), "lamps/yield[bnLight]");
    addParameterRef<double>(Name(rnLight), "lamps/yield[rnLight]");
    addParameterRef<double>(Name(Toutdoors), "outdoors/records[Tair]");
    addParameterRef<double>(Name(Tindoors), "indoors/temperature[air]");

    addParameter<double>(Name(KScreenEnergy), 5.2, "K-value for energy screen (W/m2/K)");
    addParameter<double>(Name(KCover), 7.9, "K-value for greenhouse cover (W/m2/K)");
    addParameter<double>(Name(ventilation), 0., "Greenhouse ventilation rate (m3 air/m2 greenhouse/s");
    addParameter<double>(Name(stdVentilation), 0., "Greenhouse ventilation rate normalised to a wind speed of 4 m/s (m3 air/m2 greenhouse/s");
    addParameter<double>(Name(floorHec), 6., "Heat capacity of the floor (W/m2/K)");
    addParameter<double>(Name(LEtot), 0., "Crop latent heat flux Weihong/Goudriaan (W/m2");
    addParameter<double>(Name(dewCondensation), 0., "Dew condensation rate (kg/m2/s");
    addParameter<double>(Name(lampHeat), 0., "Convective heat from lamps (W/m2)");
    addParameter<double>(Name(spHeating), 0., "Setpoint for heating (oC)");
    addVariable<double>(Name(heatingDemand), "Energy demand for heating (W/m2)");
    addVariable<double>(Name(Tunheated), "Greenhouse temperature if not heated (oC)");
    addVariable<double>(Name(energyBalance), "Energy balance of greenhouse (W/m2)");
    addVariable<double>(Name(heatCapacity), "Heat capacity of greenhouse (W/m2/K)");
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

