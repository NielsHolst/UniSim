/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "crop_radiation_absorption.h"
#include "general.h"

using namespace UniSim;

namespace vg {
	
CropRadiationAbsorption::CropRadiationAbsorption(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(Tindoors), "climate/temperature[value]");
    addParameterRef<double>(Name(diffuseRadiation), "climate/radiation[diffuse]");
    addParameterRef<double>(Name(screenTransmission), "regulation/screens[lightTransmission]");
    addParameterRef<double>(Name(lampLongWaveEmission), "regulation/light/attributes[longWaveEmission]");
    addParameterRef<double>(Name(lampShortWaveEmission), "regulation/light/attributes[shortWaveEmission]");
    addVariable<double>(Name(longWave),"Long wave absorption (thermal radiation, Bn) (W/m2)");
    addVariable<double>(Name(shortWave),"Short wave absorption (visible light, Rn) (W/m2)");
}

void CropRadiationAbsorption::reset() {
    longWave = shortWave = 0.;
}

void CropRadiationAbsorption::update() {
    const double Emiss = 0.95;
    double Tshelter = Tscreen*screenTransmission + Tcover*(1. - screenTransmission);
    double diff = pow(Tindoors + T0, 4) - pow(Tshelter + T0, 4);
    longWave = Emiss*Sigma*diff + lampLongWaveEmission;
    shortWave = 0.66*(diffuseRadiation + lampShortWaveEmission);
}

} //namespace

