/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "greenhouse_construction.h"
#include "lamp_attributes.h"

using namespace UniSim;

namespace vg {
	
LampAttributes::LampAttributes(Identifier name, QObject *parent)
    : Model(name, parent)
{
    addParameterRef<int>(Name(lampType), "construction[iLampType]");
    addParameter<double>(Name(capacity), 40., "Capacity of installed lamps (W/m2)");
    addParameter<double>(Name(ballastCorrection), 1., "Set to >1 (e.g, 1.15) if @F capacity includes ballast");

    addVariable<double>(Name(heatEmission),"Convective heat emission (W/m2)");
    addVariable<double>(Name(longWaveEmission),"Long wave emission (thermal radiation, Bn) (W/m2)");
    addVariable<double>(Name(shortWaveEmission),"Short wave emission (visible light, Rn) (W/m2)");
    addVariable<double>(Name(parEmission),"PAR radiation (W/m2)");
    addVariable<double>(Name(energyUse),"Energy used (W/m2)");
}

void LampAttributes::initialize() {
    switch (GreenhouseConstruction::GlassType(lampType)) {
    case GreenhouseConstruction::HPSL:
        heatCoef = 0.23;
        longWaveCoef = 0.42;
        shortWaveCoef = 0.50;
        parCoef = 0.31;
        break;

    case GreenhouseConstruction::LED:
        heatCoef = 0.02;
        longWaveCoef = 0.05;
        shortWaveCoef = 0.82;
        parCoef = 0.82;
        break;

    default:
        throw Exception(QString("Unknown lamp type number: %1").arg(lampType), this);
    }
}

void LampAttributes::reset() {
    double netCapacity = capacity/ballastCorrection;
    heatEmission = heatCoef*netCapacity;
    longWaveEmission = longWaveCoef*netCapacity;
    shortWaveEmission = shortWaveCoef*netCapacity;
    parEmission = parCoef*netCapacity;
    energyUse = ballastCorrection*capacity;
}

} //namespace

