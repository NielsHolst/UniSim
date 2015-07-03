/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_AIR_FLUX_GRAVITATION_H
#define VG_AIR_FLUX_GRAVITATION_H

#include <usbase/model.h>

namespace vg {

class AirFluxGravitation : public UniSim::Model
{
public:
    AirFluxGravitation(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double state, topTemperature, bottomTemperature, topVolume, bottomVolume, greenhouseArea, timeStep;

    // Outputs
    double value;

    // Methods
    double maxFiniteRate(double finiteRate1) const;
};
} //namespace


#endif
