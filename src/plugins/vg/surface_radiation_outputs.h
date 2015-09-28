/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SURFACE_RADIATION_OUTPUTS_H
#define VG_SURFACE_RADIATION_OUTPUTS_H

#include "surface_radiation.h"
#include <usbase/model.h>

namespace vg {


class SurfaceRadiationOutputs : public UniSim::Model
{
public:
    SurfaceRadiationOutputs(UniSim::Identifier name, QObject *parent);
    void resetRadiationOutputs() ;
    void set(const SurfaceRadiation &rad);
    const SurfaceRadiation* surfaceRadiation() const;
private:
    // Outputs
    double lightTransmissivity, directLightTransmissivity, irTransmissivity,
           incomingLightAbsorptivity, incomingLightReflectivity,
           incomingDirectLightAbsorptivity, incomingDirectLightReflectivity,
           incomingLwAbsorptivity, incomingLwReflectivity,
           outgoingLightAbsorptivity, outgoingLightReflectivity,
           outgoingDirectLightAbsorptivity, outgoingDirectLightReflectivity,
           outgoingLwAbsorptivity, outgoingLwReflectivity;
    // Data
    SurfaceRadiation rad;

};
} //namespace


#endif
