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
    double inflowTemperatureNeeded(double effect);

private:
    // Parameters
    double length, diameter, flowRate, greenhouseArea,
        inflowTemperature, indoorsTemperature, timeStep;

    // Variables
    double temperature, effect, propFlow;

    // Methods
    void updateFlow();
    double specificEffect(double Tpipe);
    double pipeTemperature(double specificEffect);
    void selfTest();

    // Data
    const double exponent{1.25};
    double slope,
        volume;   // Volume of pipe water per m2 greenhouse [kg = L]
};
} //namespace


#endif
