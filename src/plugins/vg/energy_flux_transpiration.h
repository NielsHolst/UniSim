/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_ENERGY_FLUX_TRANSPIRATION_H
#define VG_ENERGY_FLUX_TRANSPIRATION_H

#include <usbase/model.h>

namespace vg {

class EnergyFluxTranspiration : public UniSim::Model
{
public:
    EnergyFluxTranspiration(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    double transpiration, flux;
};
} //namespace


#endif
