/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
//#include <usbase/name.h>
#include "crop_layer.h"
#include "general.h"

using namespace UniSim;

namespace vg {
	
CropLayer::CropLayer(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(Tindoors), "climate/temperature[value]");
    addParameterRef<double>(Name(mcIndoors), "climate/humidity[mc]");
    addParameterRef<double>(Name(vpIndoors), "climate/humidity[vp]");
    addParameterRef<double>(Name(vpdIndoors), "climate/humidity[vpd]");
    addParameterRef<double>(Name(diffuseRadiation), "climate/radiation[diffuse]");
    addParameterRef<double>(Name(cropLai), "crop/growth[lai]");
    addParameterRef<double>(Name(lowerPipeHeatFlux), "pipes/lower[heatFlux]");
    addParameterRef<double>(Name(kDiff), "crop[kDiff]");
    addParameterRef<double>(Name(riH2O), "crop/riH2O[value]");
    addParameterRef<double>(Name(rbH2O), "crop/rbH2O[value]");
    addParameterRef<double>(Name(longWaveAbsorption), "crop/radiationAbsorption[longWave]");
    addVariable<double>(Name(absorbedRadiation), "Radiation absorbed by crop layer (W/m2)");
    addVariable<double>(Name(temperature), "Temperature of crop layer (oC)");
    addVariable<double>(Name(surfaceMoistureContent), "Air moisture content at leaf surface (g/m3)");
    addVariable<double>(Name(transpirationConductance), "Transpiration conductance of leaves (m/s)");
    addVariable<double>(Name(latentHeatFlux), "Latent heat flux of crop layer (W/m2)");
//    addVariable<double>(Name(vapourFlux), "Vapour flux from crop layer (g/m2/s)");
//    addVariable<double>(Name(dewVapourFlux), "Vapour flux from dew in crop layer (g/m2/s)");
    addVariable<double>(Name(dew), "Dew formation rate on crop (g/m2/s)");
}

void CropLayer::initialize() {
    ixLayer = seekSiblingPosition<CropLayer*>();
    numLayers = seekSiblings<CropLayer*>("*").size() + 1;
}

void CropLayer::reset() {
    temperature = 20.;
    surfaceMoistureContent = latentHeatFlux = vapourFlux = dewVapourFlux = dew = 0.;
}

void CropLayer::update() {
    lai = cropLai/numLayers;
    laiAbove = double(ixLayer)*cropLai/numLayers;
    double netRadiation = diffuseRadiation*(exp(-kDiff*laiAbove));
    absorbedRadiation = 0.66*netRadiation + 0.95*Sigma*pow(1.06*Tindoors-21, 4);

    s = svpSlope(Tindoors);
    updateTemperature();
    updateLatentHeatFlux();
    updateVapourFlux();
}

void CropLayer::updateTemperature() {
    double pipeHeat = 0.025*lowerPipeHeatFlux*exp(laiAbove);
    double a = 1./(RhoAir*CpAir)*(riH2O+rbH2O)*(pipeHeat + absorbedRadiation - longWaveAbsorption)
                - 1./Psychr*vpdIndoors;
    double b = 1. + s/Psychr + riH2O/rbH2O + 4*Sigma*pow(Tindoors+T0,3)/RhoAir/CpAir*(riH2O+rbH2O);
    temperature = a/b + Tindoors;
}

// GCC eq. 3.4.12
void CropLayer::updateLatentHeatFlux() {
    double vpdCropAir = svp(temperature) - vpIndoors;
    latentHeatFlux = (s*absorbedRadiation + 2*lai*RhoAir*CpAir*vpdCropAir/rbH2O) /(s + Psychr*(1+riH2O/rbH2O));
}

void CropLayer::updateVapourFlux() {
    double smc = saturatedMoistureContent(Tindoors);                                // g/m3
    surfaceMoistureContent = smc + s/Psychr*rbH2O/2./lai*absorbedRadiation/LHe; // g/m3
    transpirationConductance = 2*lai/((1. + s/Psychr)*rbH2O + riH2O);   // m/s
    vapourFlux = transpirationConductance*(surfaceMoistureContent - mcIndoors)/1000.;    // kg/m2/s
}

void CropLayer::updateDewVapourFlux() {
    double smc = saturatedMoistureContent(temperature);
    double effectiveHumidity = smc + RhoAir/Psychr*rbH2O/2*lai*absorbedRadiation/LHe;
    double transpirationConductance = 2*lai/((1. + s/Psychr)*rbH2O);
    dewVapourFlux = transpirationConductance*(effectiveHumidity - mcIndoors)/1000.;           // kg/m2/s
}

void CropLayer::updateDew() {
    const double gcnd = 3e-3; //  m/s
    double smc = saturatedMoistureContent(temperature);
    dew = (mcIndoors < smc) ? 0. : gcnd*(mcIndoors - smc)*2.*lai;
}

} //namespace

