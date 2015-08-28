/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SHELTER_H
#define VG_SHELTER_H

#include "surface_radiation_outputs.h"

namespace UniSim {
    class DataGrid;
}

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
    QString directTransmissionFile;
    double latitude, azimuth, greenhouseShade, chalk,
    coverAreaRoof, coverAreaSideWalls, coverAreaEndWalls, coverAreaGables,
    outdoorsDirectRadiation, outdoorsDiffuseRadiation;

    // Outputs
    double area, diffuseLightTransmitted, directLightTransmitted, totalLightTransmitted,
        airTransmissivity, haze, U, maxScreenState;

    // Data
    double relativeArea;
    const double *pLightTransmissivity,
        *pCoverU, *pCoverHaze, *pCoverDiffuseTransmission,
        *pScreensU, *pScreensAirTransmission, *pScreensHaze, *pMaxScreenState;
    const SurfaceRadiation *pCoverSurfaceRadiation, *pScreensSurfaceRadiation;
    UniSim::DataGrid *dirTransTable;

    // Methods
    void updateU();
    void updateHaze();
    void updateAirTransmission();
    void updateLightTransmission();

};
} //namespace


#endif
