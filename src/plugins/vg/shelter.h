/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SHELTER_H
#define VG_SHELTER_H

#include "surface_radiation_outputs.h"

namespace vg {

class Shelter : public SurfaceRadiationOutputs
{
public:
    Shelter(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
    void update();

private:
    // Inputs
    double greenhouseShade, chalk,
    coverAreaRoof, coverAreaSideWalls, coverAreaEndWalls, coverAreaGables, groundArea,
    outdoorsDirectRadiation, outdoorsDiffuseRadiation;

    // Outputs
    double area, relativeArea, areaPerGround,
        diffuseLightTransmitted, directLightTransmitted, totalLightTransmitted,
        lightAbsorbedCover, lightAbsorbedScreens,
        airTransmissivity, haze, U, maxScreenState;

    // Data
    const double
        *pCoverU, *pCoverHaze,
        *pScreensU, *pScreensHaze,
        *pScreensAirTransmission,
        *pMaxScreenState;

    struct Light {
        struct {
            const double *tra, *abs;
        } diffuse, direct;
        void fetch(UniSim::Model *model);
    };
    Light shelter, cover, screens;

    const SurfaceRadiation *pCoverSurfaceRadiation, *pScreensSurfaceRadiation;

    // Methods
    void updateU();
    void updateHaze();
    void updateAirTransmission();
    void updateLightTransmission();

};
} //namespace


#endif
