/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SHELTER_H
#define VG_SHELTER_H

#include "shelter_base.h"

namespace vg {

class Shelter : public ShelterBase
{
public:
    Shelter(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
    void update();

private:
    // Inputs
    double greenhouseShade, chalk,
    roofArea, sideWallsArea, endWallsArea, gablesArea, groundArea,
    outdoorsDirectRadiation, outdoorsDiffuseRadiation;

    // Outputs
    double area, relativeArea, areaPerGround;

    // Data
    const double
        *pCoverU, *pCoverHaze,
        *pScreensU, *pScreensHaze,
        *pScreensAirTransmission;

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
