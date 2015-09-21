/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_INDOORS_CO2_H
#define VG_INDOORS_CO2_H

#include <usbase/model.h>

namespace vg {

class IndoorsCo2 : public UniSim::Model
{
public:
    IndoorsCo2(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double outdoorsCo2, airFlux,
        assimilation, indoorsTemperature,
        averageHeight, timeStep,
        injectionRate;
    // Outputs
    double value;
    // Methods
    double ppmToDensity(double ppm);
    double densityToPpm(double density);
};
} //namespace


#endif
