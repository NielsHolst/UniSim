/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_ENERGY_FLUX_VENTILATION_H
#define VG_ENERGY_FLUX_VENTILATION_H

#include <usbase/model.h>

namespace vg {

class EnergyFluxVentilation : public UniSim::Model
{
public:
    EnergyFluxVentilation(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    double relativeVentilationRate, indoorsTemperature, outdoorsTemperature,
        timeStep, averageHeight,
        flux;
};
} //namespace


#endif
