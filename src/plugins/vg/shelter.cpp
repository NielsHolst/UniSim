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
 * - _roofArea_ is the total area of the roof (the two sloping surfaces on top of each span) [m<SUP>2</SUP>]
 * - _sideWallsArea_ is the total area of the two greenhouse side walls (facing the outside) [m<SUP>2</SUP>]
 * - _endWallsArea_ is the total area of the two greenhouse end walls (excluding the triangular gables) [m<SUP>2</SUP>]
 * - _gablesArea_ is the total area of the two triangular gables at the ends of each span [m<SUP>2</SUP>]
 * - _groundArea_ is the area covered by the greenhouse [m<SUP>2</SUP>]
 * - _outdoorsDirectRadiation_ is the direct component of sunlight irradiation [W/m<SUP>2</SUP>]
 * - _outdoorsDiffuseRadiation_ is the diffuse component of sunlight irradiation [W/m<SUP>2</SUP>]
 *
 * Outputs
 * -------
 * - _area_ is the surface area [m<SUP>2</SUP>]
 * - _relativeArea_ is the proportion of this surface out of the total surface area [0;1]
 * - _areaPerGround_ is the surface area relative to the ground area [m<SUP>2</SUP> shelter/m<SUP>2</SUP> ground]
 */

Shelter::Shelter(Identifier name, QObject *parent)
    : ShelterBase(name, parent)
{
    InputRef(double, roofArea, "geometry[roofArea]");
    InputRef(double, sideWallsArea, "geometry[sideWallsArea]");
    InputRef(double, endWallsArea, "geometry[endWallsArea]");
    InputRef(double, gablesArea, "geometry[gablesArea]");
    InputRef(double, groundArea, "geometry[groundArea]");

    InputRef(double, outdoorsDirectRadiation, "outdoors[directRadiation]");
    InputRef(double, outdoorsDiffuseRadiation, "outdoors[diffuseRadiation]");
    InputRef(double, screensMaxState, "./screens[maxState]");

    Output(double, area);
    Output(double, relativeArea);
    Output(double, areaPerGround);
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
    pScreensAirTransmission = screensM->pullValuePtr<double>("airTransmissivity");
}

void Shelter::Light::fetch(UniSim::Model *model) {
    diffuse.tra = model->pullValuePtr<double>("lightTransmissivity");
    direct.tra = model->pullValuePtr<double>("directLightTransmissivity");
    diffuse.abs = model->pullValuePtr<double>("incomingLightAbsorptivity");
    direct.abs = model->pullValuePtr<double>("incomingDirectLightAbsorptivity");
}

void Shelter::reset() {
    ShelterBase::reset();

    QString name = id().label();
    if (name.toLower().contains("roof"))
        area = roofArea/2;
    else if (name.toLower().contains("side"))
        area = sideWallsArea/2;
    else if (name.toLower().contains("end"))
        area = endWallsArea/2;
    else {
        QString msg{"Cover name is '%1' but it must contain 'roof', 'side' or 'end'"};
        throw Exception(msg.arg(name));
    }
    relativeArea = area/(roofArea + sideWallsArea + endWallsArea);
    areaPerGround = area/groundArea;
}

void Shelter::update() {
    SurfaceRadiation rad = (*pCoverSurfaceRadiation);
    rad *= (*pScreensSurfaceRadiation);
    set(rad);
    updateU();
    updateHaze();
    updateAirTransmission();
    updateLightTransmission();
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

