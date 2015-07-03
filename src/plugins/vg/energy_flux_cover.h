/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_ENERGY_FLUX_COVER_H
#define VG_ENERGY_FLUX_COVER_H

#include "energy_flux_base.h"

namespace vg {

class EnergyFluxCover : public EnergyFluxBase
{
public:
    EnergyFluxCover(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double U, emissivity, absorptivity, density, heatCapacity, thickness,
        timeStep, averageHeight, indoorsTemperature, skyTemperature, outdoorsTemperature,
        screenState, cropTemperature,
        sunlight, condensationRate;

    // Outputs
    double temperature, heatFluxOutside, heatFluxInside,
        radiationFluxSky, radiationFluxSun, radiationFluxScreen, radiationFluxCrop;

    // Methods
    void doUpdate();


};
} //namespace


#endif
