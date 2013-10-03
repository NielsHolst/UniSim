/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "crop.h"

using namespace UniSim;

namespace vg {
	
Crop::Crop(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<double>(Name(kdiff), 0.8, "Extinction coefficient for diffuse light (-)");
    addParameter<double>(Name(coverage), 0.1, "Proportion of floor covered by crop [0;1]");
    addVariable<double>(Name(absorbedRadiation), "Total radiation absorbed by crop (W/m2)");
    addVariable<double>(Name(temperature), "Average temperature of crop (oC)");
    addVariable<double>(Name(surfaceMoistureContent), "Average air moisture content at leaf surface (g/m3)");
    addVariable<double>(Name(transpirationConductance), "Total transpiration conductance of leaves (m/s)");
    addVariable<double>(Name(latentHeatFlux), "Average latent heat flux of crop (W/m2)");
//    addVariable<double>(Name(vapourFlux), "Total vapour flux from crop (g/m2/s)");
//    addVariable<double>(Name(dewVapourFlux), "Total vapour flux from dew in crop (g/m2/s)");
    addVariable<double>(Name(dew), "Total dew formation rate on crop (kg/m2/s)");
}

void Crop::initialize() {
    ar.set(this, "crop/layers/*[absorbedRadiation]");
    t.set(this, "crop/layers/*[temperature]");
    smc.set(this, "crop/layers/*[surfaceMoistureContent]");
    tc.set(this, "crop/layers/*[transpirationConductance]");
    lhf.set(this, "crop/layers/*[latentHeatFlux]");
//    vf.set(this, "crop/layers/*[vapourFlux]");
//    dvf.set(this, "crop/layers/*[dewVapourFlux]");
    d.set(this, "crop/layers/*[dew]");
}

void Crop::reset() {
    temperature = 20.;
    surfaceMoistureContent = transpirationConductance = absorbedRadiation  = latentHeatFlux
            = vapourFlux = dewVapourFlux = dew = 0.;
}

void Crop::update() {
    temperature = t.average();
    surfaceMoistureContent = smc.average();
    transpirationConductance = tc.sum();
    absorbedRadiation = ar.sum();
    latentHeatFlux = lhf.sum();
//    vapourFlux = vf.sum();
//    dewVapourFlux = dvf.sum();
    dew = d.sum();
}

} //namespace

