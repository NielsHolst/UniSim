/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SHELTER_BASE_H
#define VG_SHELTER_BASE_H

#include "surface_radiation_outputs.h"

namespace vg {

class ShelterBase : public SurfaceRadiationOutputs
{
public:
    ShelterBase(UniSim::Identifier name, QObject *parent);
    void reset();

protected:
    // Outputs
    double diffuseLightTransmitted, directLightTransmitted, totalLightTransmitted,
        lightAbsorbedCover, lightAbsorbedScreens,
        haze, U,
        airTransmissivity;
};
} //namespace


#endif
