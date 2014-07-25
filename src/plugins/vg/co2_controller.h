/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_CO2_CONTROLLER_H
#define VG_CO2_CONTROLLER_H
#include <usbase/model.h>

namespace vg {

class Co2Controller : public UniSim::Model
{
public:
    Co2Controller(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    double assimilation, ventilation, minCo2, indoorsCo2, outdoorsCo2, indoorsTemperature,
        timeStep, height, maxSignal,
        signal;
};
} //namespace


#endif
