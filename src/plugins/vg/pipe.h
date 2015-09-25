/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_PIPE_H
#define VG_PIPE_H

#include <usbase/model.h>

namespace vg {

class Pipe : public UniSim::Model
{
public:
    Pipe(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

    double calcEnergyFlux(double temperatureDifference) const;
    double calcTemperatureDifference(double energyFlux) const;
    double minEnergyFlux() const;
private:
    // Inputs
    double length, diameter, minTemperature, maxTemperature, maxTemperatureIncreaseRate,
        energyFlux, indoorsTemperature, timeStep;
    // Outputs
    double temperature,
        nextTemperatureMin, nextTemperatureMax,
        nextEnergyFluxMin, nextEnergyFluxMax;

    // Data
    const double exponent{1.25};
    double slope;
};

} //namespace


#endif
