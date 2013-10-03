/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include <usbase/exception.h>
#include "general.h"
#include "greenhouse_energetics.h"

using namespace UniSim;

namespace vg {
	
GreenhouseEnergetics::GreenhouseEnergetics(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(coverDew), "greenhouse/cover/dew[value]");
    addParameterRef<double>(Name(screenDew), "regulation/screens[dew]");
    addParameterRef<double>(Name(cropDew), "crop[dew]");
    addParameterRef<double>(Name(ventilation), "greenhouse/ventilation[value]");
    addParameterRef<double>(Name(energyScreenState), "screens/energy/control[state]");
    addParameterRef<double>(Name(longWaveLight), "light/attributes[longWaveEmission]");
    addParameterRef<double>(Name(shortWaveLight), "light/attributes[shortWaveEmission]");
    addParameterRef<double>(Name(lampHeat), "light/attributes[heatEmission]");
    addParameterRef<double>(Name(Toutdoors), "environment[temperature]");
    addParameterRef<double>(Name(Tfloor), "floor/temperature[state]");
    addParameterRef<double>(Name(cropLatentHeatFlux), "crop[latentHeatFlux]");

    addParameter<double>(Name(KScreenEnergy), 5.2, "K-value for energy screen (W/m2/K)");
    addParameter<double>(Name(KCover), 7.9, "K-value for greenhouse cover (W/m2/K)");
    addParameter<double>(Name(floorHec), 6., "Heat capacity of the floor (W/m2/K)");

    addVariable<double>(Name(energyBalance), "Energy balance of greenhouse (W/m2)");
    addVariable<double>(Name(heatCapacity), "Heat capacity of greenhouse (W/m2/K)");
    addVariable<double>(Name(Tunheated), "Greenhouse temperature if not heated (oC)");
    addVariable<double>(Name(dewHeat), "Heat generated from dew condensation (W/m2)");
    addVariable<double>(Name(outdoorsHeat), "Heat from outside (W/m2)");
    addVariable<double>(Name(floorHeat), "Heat from floor (W/m2)");
}

void GreenhouseEnergetics::reset() {
    double Tindoors(20.), Toutdoors(20.);
    energyScreenState = 0.;
    energyBalance = KTotal()*(Toutdoors+T0) + floorHec*(Tindoors+T0);
    heatCapacity  = KTotal() + floorHec;
    updateTunheated();
}

void GreenhouseEnergetics::update() {
    outdoorsHeat = (KTotal() + ventilation*CpAir)*(Toutdoors+T0);
    dewHeat = LHe*(screenDew + coverDew + cropDew)/1000.;
    floorHeat = floorHec*(Tfloor+T0);
    energyBalance =
        outdoorsHeat - cropLatentHeatFlux + dewHeat + lampHeat + shortWaveLight - longWaveLight + floorHeat;
    heatCapacity  = KTotal() + ventilation*CpAir + floorHec;
    updateTunheated();
}

double GreenhouseEnergetics::KTotal() const {
    return (1-energyScreenState)*KCover + energyScreenState*KScreenEnergy;
}

void GreenhouseEnergetics::updateTunheated() {
    Tunheated = energyBalance/heatCapacity - T0;
    if (Tunheated < -T0 || Tunheated > 100) {
        QString msg = "Tunheated=%1 outside reasonable range (energyBalance=%2, heatCapacity=%3";
        throw Exception(msg.arg(Tunheated).arg(energyBalance).arg(heatCapacity), this);
    }
}


} //namespace

