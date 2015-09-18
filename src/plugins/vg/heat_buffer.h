/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_HEAT_BUFFER_H
#define VG_HEAT_BUFFER_H

#include <usbase/model.h>

namespace vg {

class HeatBuffer : public UniSim::Model
{
public:
    HeatBuffer(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double volume, minTemperature, maxTemperature, U, heatingEnergyFlux, indoorsTemperature, timeStep;

    // Outputs
    double temperature, energyFlux, energyFluxLag;

};
} //namespace


#endif