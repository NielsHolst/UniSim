/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_ENERGY_FLUX_SHELTERS_H
#define VG_ENERGY_FLUX_SHELTERS_H

#include "energy_flux_base.h"

namespace vg {

class EnergyFluxShelter : public EnergyFluxBase
{
public:
    EnergyFluxShelter(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double U, incomingIrAbsorptivity, incomingIrAbsorptivityScreens,
        outgoingIrAbsorptivity, outgoingIrAbsorptivityCover, outgoingIrAbsorptivityScreens,
        lightAbsorbedCover, lightAbsorbedScreens,
        heatCapacityCover, heatCapacityScreens,
        timeStep, averageHeight, coverPerGroundArea, relativeShelterArea,
        indoorsTemperature, skyTemperature, outdoorsTemperature,
        maxState,
        cropTemperatureTop, cropTemperatureMiddle, cropTemperatureBottom,
        sunlight, condensationRateCover;

    // Outputs
    double coverTemperature, screensTemperature,
        heatFluxOutside, heatFluxInside,
        radiationFluxSky,
        radiationFluxSunCover, radiationFluxSunScreens,
        radiationFluxCropTop, radiationFluxCropMiddle, radiationFluxCropBottom;

    // Methods
    void doUpdate();
    double radiationFluxCrop(double cropTemperature) const;


};
} //namespace


#endif
