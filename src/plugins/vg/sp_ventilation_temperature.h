/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SP_VENTILATION_TEMPERATURE_H
#define VG_SP_VENTILATION_TEMPERATURE_H

#include <usbase/model.h>

namespace vg {

class SpVentilationTemperature : public UniSim::Model
{
public:
    SpVentilationTemperature(UniSim::Identifier name, QObject *parent);
    void update();
private:
    // Parameters
    double spHeating, heatingIncrement, humidityDecrement;

    // Variables
    double sp;
};
} //namespace


#endif
