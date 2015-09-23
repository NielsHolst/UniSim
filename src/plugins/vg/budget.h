/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_BUDGET_H
#define VG_BUDGET_H

#include <usbase/model.h>

namespace vg {

class Budget : public UniSim::Model
{
public:
    Budget(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double heatingEnergyFlux, growthLightsEnergyFlux, co2Flux, dt;

    // Outputs
    double heatingEnergyTotal, growthLightsEnergyTotal, co2Total;

};
} //namespace


#endif
