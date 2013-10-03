/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SHADE_SCREEN_CONTROLLER_H
#define VG_SHADE_SCREEN_CONTROLLER_H

#include <usbase/model.h>

namespace vg {

class ShadeScreenController : public UniSim::Model
{
public:
    ShadeScreenController(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
    void update();
private:
    // Parameters
    double radiationThreshold, temperatureThreshold, radiation, temperature, maxSignalHighHumidity, tolerance;
    bool isHumidityHigh, followEnergyScreen;

    // Variable
    double signal;

    // Links
    const double *energyScreenSignal;
};
} //namespace


#endif
