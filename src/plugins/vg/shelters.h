/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SHELTERS_H
#define VG_SHELTERS_H

#include "shelter_base.h"

namespace vg {

class SurfaceRadiation;

class Shelters : public ShelterBase
{
public:
    Shelters(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
    void update();

private:
    // Inputs
    double groundArea;
    // Outputs
    double heatCapacityCoversPerGround, heatCapacityScreensPerGround,
        screensEffectiveArea, screensPerGroundArea, screensMaxState;
    // Data
    struct ShelterInfo {
        const SurfaceRadiation *sr;
        const double *diffuseLightTransmitted, *directLightTransmitted, *totalLightTransmitted,
            *lightAbsorbedCover, *lightAbsorbedScreens,
            *haze, *U,
            *airTransmissivity,
            *area, *relativeArea, *screensEffectiveArea,
            *heatCapacityCover, *heatCapacityScreens,
            *screensMaxState;
    };

    QVector<ShelterInfo> infos;
};
} //namespace


#endif
