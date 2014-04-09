/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_VENTILATION_CONTROLLER_H
#define VG_VENTILATION_CONTROLLER_H

#include <usbase/model.h>

namespace vg {

class VentilationController : public UniSim::Model
{
public:
    VentilationController(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    // Parameters
    double windSideProportion, Tventilation,
        windSpeed, indoorsTemperature, outdoorsTemperature,
        maxSignal, byTemperature, byHumidity, byTemperatureDiff;
    // Variables
    double leeSideSignal, windSideSignal;
    // Methods
    double windFactor();
};
} //namespace


#endif
