/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_ENERGY_SCREEN_CONTROLLER_H
#define VG_ENERGY_SCREEN_CONTROLLER_H

#include <usbase/model.h>

namespace vg {

class EnergyScreenController : public UniSim::Model
{
public:
    EnergyScreenController(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
    void update();
private:
    // Parameters
    double radiationThreshold, radiation, maxSignalHighHumidity;
    bool isHumidityHigh;

    // Variable
    double signal;

    // Links
    const double *energyBalance;
};
} //namespace


#endif
