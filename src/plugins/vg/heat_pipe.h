/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_HEAT_PIPE_H
#define VG_HEAT_PIPE_H

#include <usbase/model.h>
#include <usbase/string_map.h>

namespace vg {

class HeatPipe : public UniSim::Model
{
public:
    HeatPipe(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
    void update();
    // Special methods
    double temperatureNeeded(double effect);

private:
    // Parameters
    double length, diameter, flow, pumpControl,
        inflowTemperature, indoorsTemperature, timeStepSecs;

    // Variables
    double temperature, energyEmitted;

    // Methods
    double effect(double Tpipe);
    void selfTest();

    // Data
    const double a{0.0055};
    double b, propFlow, volume;
};
} //namespace


#endif
