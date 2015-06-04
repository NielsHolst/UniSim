/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_HEAT_PIPE_H
#define VG_HEAT_PIPE_H

#include <usbase/model.h>
#include <usbase/string_map.h>

namespace vg {

class HeatPipe : public UniSim::Model
{
public:
    HeatPipe(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
    // Special methods
    double calcEnergyFluxMin() const;
    double calcEnergyFluxMax() const;
    double calcEnergyFlux(double pipeTemperature) const;
    double calcPipeTemperature(double energyFlux) const;

private:
    // Inputs
    double length, diameter, minTemperature, maxTemperature, indoorsTemperature, energyFlux;

    // Outputs
    double temperature, minEnergyFlux, maxEnergyFlux;

    // Data
    const double exponent{1.25};
    double slope;
};

} //namespace


#endif
