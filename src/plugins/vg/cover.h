/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_COVER_H
#define VG_COVER_H

#include <usbase/model.h>
#include "surface_radiation_outputs.h"

namespace vg {

class Cover : public vg::SurfaceRadiationOutputs
{
public:
    Cover(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double U4, windspeed, emissivity, absorptivity, transmissivity, heatCapacity, haze;
    bool antiReflection;

    // outputs
    double U;
};
} //namespace


#endif
