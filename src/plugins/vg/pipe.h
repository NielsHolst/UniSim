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
    void initialize();
    void reset();
    void update();
private:
    // Inputs
    double length, diameter, minTemperature, maxTemperature, maxTemperatureIncreaseRate, emissivity,
        energyFluxTotal, indoorsTemperature, timeStep;
    // Outputs
    double temperature, energyFlux,
        nextTemperatureMin, nextTemperatureMax,
        nextEnergyFluxMin, nextEnergyFluxMax;

    // Data
    const double exponent{1.25};
    double slope;
    QVector<const double*> energyFluxFromPreceedingPipes;

    // Methods
    QList<UniSim::Model*> seekPrecedingSiblings();
    double calcEnergyFlux(double temperatureDifference) const;
    double calcTemperatureDifference(double energyFluxTotal) const;
    void setNextTemperatureMin();
    double energyFluxFromPreceedingPipesSum();
};

} //namespace


#endif
