/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_TEMPERATURE_SETPOINTS_H
#define VG_TEMPERATURE_SETPOINTS_H

#include <usbase/model.h>

namespace vg {

class TemperatureSetpoints : public UniSim::Model
{
public:
    TemperatureSetpoints(UniSim::Identifier name, QObject *parent);
    void update();
private:
    // Parameters
    double setMinimum, setMaximum, humidityIncrement, humidityDecrement;

    // Variables
    double minimum, maximum;
};
} //namespace


#endif
