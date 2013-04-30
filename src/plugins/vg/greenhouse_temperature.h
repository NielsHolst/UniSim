/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_GREENHOUSE_TEMPERATURE_H
#define VG_GREENHOUSE_TEMPERATURE_H

#include <usbase/model.h>

namespace vg {

class GreenhouseTemperature : public UniSim::Model
{
public:
    GreenhouseTemperature(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double energyBalance, heatCapacity, pipesHeatTransfer;

    // Variables
    double temperature;

};
} //namespace


#endif
