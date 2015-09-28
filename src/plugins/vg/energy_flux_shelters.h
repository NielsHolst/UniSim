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

class EnergyFluxShelters : public EnergyFluxBase
{
public:
    EnergyFluxShelters(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double U, incomingLwAbsorptivity,
        outgoingLwAbsorptivity,
        lightAbsorbedCover, lightAbsorbedScreens,
        heatCapacityCover, heatCapacityScreens,
        timeStep, averageHeight,
        coverPerGroundArea,
        indoorsTemperature, skyTemperature, outdoorsTemperature,
        radiationFluxCropTop, radiationFluxCropMiddle, radiationFluxCropBottom;

    // Outputs
    double coverTemperature, screensTemperature,
        heatFluxOutsideToCover, heatFluxInsideToCover,
        radiationFluxSkyToCover,
        radiationFluxSunToCover, radiationFluxSunToScreens;

};
} //namespace


#endif
