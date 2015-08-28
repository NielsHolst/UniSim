/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/data_grid.h>
#include <usbase/interpolate.h>
#include <usengine/simulation.h>
#include "publish.h"
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
    : ShelterOutputs(name, parent)
{
    Input(QString, directTransmissionFile, "direct_transmission_single.txt");
    InputRef(double, latitude, "calendar[latitude]");
    InputRef(double, azimuth, "calendar[azimuth]");
    InputRef(double, greenhouseShade, "construction/geometry[shade]");
    InputRef(double, chalk, "controllers/chalk[signal]");
}

void Shelter::initialize() {
    dirTransTable = new DataGrid(simulation()->inputFilePath(directTransmissionFile), this);
    Model *cover = seekOneChild<Model*>("cover");
    pCoverU = cover->pullValuePtr<double>("U");
    pCoverHaze = cover->pullValuePtr<double>("haze");
    pCoverDiffuseTransmission = cover->pullValuePtr<double>("transmissivity");

    Model *screensModel = seekOneChild<Model*>("screens");
    pScreensU = screensModel->pullValuePtr<double>("U");
    pScreensAirTransmission = screensModel->pullValuePtr<double>("airTransmissivity");
    pScreensLightTransmission = screensModel->pullValuePtr<double>("lightTransmissivity");
    pScreensHaze = screensModel->pullValuePtr<double>("haze");;
}

void Shelter::localReset() {
    U = *pCoverU;
    airTransmissivity = 1.;
    haze = *pCoverHaze;
    diffuseLightTransmission = *pCoverDiffuseTransmission;
    directLightTransmissionAsDirect = *pCoverDiffuseTransmission/2;
    directLightTransmissionAsDiffuse = *pCoverDiffuseTransmission/2;
}

void Shelter::update() {
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
    diffuseLightTransmission = (*pCoverDiffuseTransmission) * (*pScreensLightTransmission) *
                               (1-chalk) * (1-greenhouseShade);

    double timeAndPlace = interpolate(*dirTransTable, latitude, azimuth);
    double directLightTransmission = diffuseLightTransmission*timeAndPlace;
    directLightTransmissionAsDirect = directLightTransmission*(1-haze);
    directLightTransmissionAsDiffuse = directLightTransmission*haze;
}

} //namespace

