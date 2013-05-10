/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_GREENHOUSE_VENTILATION_H
#define VG_GREENHOUSE_VENTILATION_H

#include <usbase/model.h>

namespace vg {

class GreenhouseVentilation : public UniSim::Model
{
public:
    GreenhouseVentilation(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double greenhouseHeight, windowLength, roofRatio, sideRatio,
        alphaLeeSide, alphaWindSide, alphaVentilationMax, leakageVentilation, fractionWindows,
        Tindoors, Toutdoors, windspeed, screenAirTransmission;
    int timeStep;
    char timeUnit;

    // Variables
    double windows, leakage, total;

    // Methods
    double timeStepSecs() const;
};
} //namespace


#endif
