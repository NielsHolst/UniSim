/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/data_grid.h>
#include <usbase/interpolate.h>
#include <usengine/simulation.h>
#include "cover.h"
#include "publish.h"
#include "screens.h"
#include "shelter.h"

using namespace UniSim;

namespace vg {

PUBLISH(Shelter)

/*! \class Shelter
 * \brief A shelter consists of the cover and any screens below the cover
 *
 * Inputs
 * ------
 * - _directTransmissionFile_ is a table with transmission values [0;1]
 * according to latitude (rows) and sun azimuth (columns)
 * - _latitude_ is the geographical latitude of the greenhouse [-180;180]
 * - _azimuth_ is the sun azimuth [-90;90]
 * - _greenhouseShade_ is the fraction of light caught by the greenhouse construction [0;1]
 * - _chalk_ is the chalk efficacy [0;1]
 */

Shelter::Shelter(Identifier name, QObject *parent)
    : SurfaceRadiationOutputs(name, parent)
{
    Input(QString, directTransmissionFile, "direct_transmission_single.txt");
    InputRef(double, latitude, "calendar[latitude]");
    InputRef(double, azimuth, "calendar[azimuth]");
    InputRef(double, greenhouseShade, "geometry[shade]");
    InputRef(double, chalk, "controllers/chalk[signal]");

    InputRef(double, coverAreaRoof, "geometry[roofArea]");
    InputRef(double, coverAreaSideWalls, "geometry[sideWallsArea]");
    InputRef(double, coverAreaEndWalls, "geometry[endWallsArea]");
    InputRef(double, coverAreaGables, "geometry[gablesArea]");

    InputRef(double, outdoorsDirectRadiation, "outdoors[directRadiation]");
    InputRef(double, outdoorsDiffuseRadiation, "outdoors[diffuseRadiation]");


    Output(double, diffuseLightTransmitted);
    Output(double, directLightTransmitted);
    Output(double, totalLightTransmitted);
    Output(double, airTransmissivity);
    Output(double, haze);
    Output(double, U);
    Output(double, area);
    Output(double, maxScreenState);
}

void Shelter::initialize() {
    pLightTransmissivity = pullValuePtr<double>("lightTransmissivity");

    dirTransTable = new DataGrid(simulation()->inputFilePath(directTransmissionFile), this);
    Cover *cover = seekOneChild<Cover*>("cover");
    pCoverU = cover->pullValuePtr<double>("U");
    pCoverHaze = cover->pullValuePtr<double>("haze");
    pCoverDiffuseTransmission = cover->pullValuePtr<double>("transmissivity");
    pCoverSurfaceRadiation = cover->surfaceRadiation();

    Screens *screensModel = seekOneChild<Screens*>("screens");
    pScreensU = screensModel->pullValuePtr<double>("U");
    pScreensAirTransmission = screensModel->pullValuePtr<double>("airTransmissivity");
    pScreensHaze = screensModel->pullValuePtr<double>("haze");
    pMaxScreenState = screensModel->pullValuePtr<double>("maxState");
    pScreensSurfaceRadiation = screensModel->surfaceRadiation();
}

void Shelter::reset() {
    resetRadiationOutputs();
    diffuseLightTransmitted = directLightTransmitted = totalLightTransmitted = 0.;
    U = *pCoverU;
    airTransmissivity = 1.;
    haze = *pCoverHaze;
    maxScreenState = 0.;

    QString name = id().label();
    if (name.toLower().contains("roof"))
        area = coverAreaRoof/2;
    else if (name.toLower().contains("side"))
        area = coverAreaSideWalls/2;
    else if (name.toLower().contains("end"))
        area = coverAreaEndWalls/2;
    else {
        QString msg{"Cover name is '%1' but it must contain 'roof', 'side' or 'end'"};
        throw Exception(msg.arg(name));
    }
    relativeArea = area/(coverAreaRoof + coverAreaSideWalls + coverAreaEndWalls);
}

void Shelter::update() {
    double transmissivity = (1-chalk) * (1-greenhouseShade);
    SurfaceRadiation rad = SurfaceRadiation().asCover(transmissivity, 0., 0.);
    rad *= (*pCoverSurfaceRadiation);
    rad *= (*pScreensSurfaceRadiation);
    set(rad);
    updateU();
    updateHaze();
    updateAirTransmission();
    updateLightTransmission();
    maxScreenState = *pMaxScreenState;
}

void Shelter::updateU() {
    double resistance = 1./(*pCoverU) + 1./(*pScreensU);
    U = 1./resistance;
}

void Shelter::updateHaze() {
    double unhazed = (1 - (*pCoverHaze)) * (1 - (*pScreensHaze));
    haze = 1 - unhazed;
}

void Shelter::updateAirTransmission() {
    airTransmissivity = *pScreensAirTransmission;
}

void Shelter::updateLightTransmission() {
    diffuseLightTransmitted = relativeArea*outdoorsDiffuseRadiation * (*pLightTransmissivity);
    double directLightTransmissivity = interpolate(*dirTransTable, latitude, azimuth) * (*pLightTransmissivity);
    double directLightTransmittedTotal = relativeArea*outdoorsDirectRadiation*directLightTransmissivity;
    directLightTransmitted = directLightTransmittedTotal*(1-haze);
    diffuseLightTransmitted += directLightTransmittedTotal*haze;
    totalLightTransmitted = diffuseLightTransmitted + directLightTransmitted;
}

} //namespace

