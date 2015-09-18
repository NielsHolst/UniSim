/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_ENERGY_FLUX_AIR_H
#define VG_ENERGY_FLUX_AIR_H

#include "energy_flux_base.h"

namespace vg {

class EnergyFluxAir : public EnergyFluxBase
{
public:
    EnergyFluxAir(UniSim::Identifier name, QObject *parent);
    void update();
private:
    // Inputs
    double airFlux, indoorsTemperature, outdoorsTemperature, height, timeStep;
};
} //namespace


#endif
