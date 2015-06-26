/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_ENERGY_FLUX_VENTILATION_H
#define VG_ENERGY_FLUX_VENTILATION_H

#include "energy_flux_base.h"

namespace vg {

class EnergyFluxVentilation : public EnergyFluxBase
{
public:
    EnergyFluxVentilation(UniSim::Identifier name, QObject *parent);
    void update();
private:
    // Inputs
    double ventilation, indoorsTemperature, outdoorsTemperature, averageHeight;
};
} //namespace


#endif
