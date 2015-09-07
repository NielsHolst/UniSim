/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_ENERGY_FLUX_GROWTH_LIGHTS_H
#define VG_ENERGY_FLUX_GROWTH_LIGHTS_H

#include "energy_flux_base.h"

namespace vg {

class EnergyFluxGrowthLights : public EnergyFluxBase
{
public:
    EnergyFluxGrowthLights(UniSim::Identifier name, QObject *parent);
    void update();

private:
    double heatEmission;
};
} //namespace


#endif
