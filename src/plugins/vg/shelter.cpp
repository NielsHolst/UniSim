/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include "cover.h"
#include "general.h"
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
    Output(double, lightAbsorbedCover);
    Output(double, lightAbsorbedScreens);
    Output(double, airTransmissivity);
    Output(double, haze);
    Output(double, U);
    Output(double, area);
    Output(double, relativeArea);
    Output(double, maxScreenState);
}

void Shelter::initialize() {
    shelter.fetch(this);

    Cover *coverM = seekOneChild<Cover*>("cover");
    pCoverU = coverM->pullValuePtr<double>("U");
    pCoverHaze = coverM->pullValuePtr<double>("haze");
    cover.fetch(coverM);
    pCoverSurfaceRadiation = coverM->surfaceRadiation();

    Screens *screensM = seekOneChild<Screens*>("screens");
    pScreensU = screensM->pullValuePtr<double>("U");
    pScreensHaze = screensM->pullValuePtr<double>("haze");
    screens.fetch(screensM);
    pScreensSurfaceRadiation = screensM->surfaceRadiation();
    pMaxScreenState = screensM->pullValuePtr<double>("maxState");
    pScreensAirTransmission = screensM->pullValuePtr<double>("airTransmissivity");
}

void Shelter::Light::fetch(UniSim::Model *model) {
    diffuse.tra = model->pullValuePtr<double>("lightTransmissivity");
    direct.tra = model->pullValuePtr<double>("directLightTransmissivity");
    diffuse.abs = model->pullValuePtr<double>("incomingLightAbsorptivity");
    direct.abs = model->pullValuePtr<double>("incomingDirectLightAbsorptivity");
}

void Shelter::reset() {
    resetRadiationOutputs();
    diffuseLightTransmitted = directLightTransmitted = totalLightTransmitted =
    lightAbsorbedCover = lightAbsorbedScreens = 0.;
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
    SurfaceRadiation rad = SurfaceRadiation().asCover(transmissivity, transmissivity, 0., 0.);
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
    diffuseLightTransmitted = relativeArea*outdoorsDiffuseRadiation * (*shelter.diffuse.tra);
    double directLightTransmittedTotal = relativeArea*outdoorsDirectRadiation * (*shelter.direct.tra);
    directLightTransmitted = directLightTransmittedTotal*(1-haze);
    diffuseLightTransmitted += directLightTransmittedTotal*haze;
    totalLightTransmitted = diffuseLightTransmitted + directLightTransmitted;

    double diffuseLightAbsorbedTotal =  relativeArea*outdoorsDiffuseRadiation * (*shelter.diffuse.abs);
    double directLightAbsorbedTotal =  relativeArea*outdoorsDirectRadiation * (*shelter.direct.abs);
    double diffuseAbsSum = (*cover.diffuse.abs) + (*screens.diffuse.abs);
    double directAbsSum = (*cover.direct.abs) + (*screens.direct.abs);
    lightAbsorbedCover = div0(*cover.diffuse.abs, diffuseAbsSum) * diffuseLightAbsorbedTotal +
                         div0(*cover.direct.abs, directAbsSum) * directLightAbsorbedTotal;
    lightAbsorbedScreens = div0(*screens.diffuse.abs, diffuseAbsSum) * diffuseLightAbsorbedTotal +
                           div0(*screens.direct.abs, directAbsSum) * directLightAbsorbedTotal;
}

} //namespace

