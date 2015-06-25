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
    Model *cover = peekOneChild<Model*>("cover");
    if (cover) {
        pCoverU = cover->peekValuePtr<double>("U");
        pCoverHaze = cover->pullValuePtr<double>("haze");
        pCoverDiffuseTransmission = cover->pullValuePtr<double>("diffuseTransmission");
    }
    else {
        zero = 0.;
        zero = 1.;
        pCoverU = &zero;
        pCoverHaze = &zero;
        pCoverDiffuseTransmission = &one;
    }
    screens = seekDescendants<Model*>("screens/*");
}

void Shelter::localReset() {
    U = *pCoverU;
    airTransmission = 1.;
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
    U = *pCoverU;
    for (auto screen : screens) {
        double screenU = screen->pullValue<double>("U"),
               screenState = screen->pullValue<double>("state");
        // Consider U as a conductance: Convert U values to resistances, add them and convert back to U
        U += (1-screenState)*U + screenState/(1/U + 1/screenU);
    }

//    U = *pCoverU;
//    double optimalTransmittance{1}, maxReduction{0};
//    for (auto screen : screens) {
//        double reduction = screen->pullValue<double>("state")*screen->pullValue<double>("energyLossReduction");
//        optimalTransmittance *= 1-reduction;
//        if (reduction > maxReduction)
//            maxReduction = reduction;
//    }
//    double optimalReduction = 1 - optimalTransmittance;
//    U = *pCoverU*(1 - (optimalReduction + maxReduction)/2);
}

void Shelter::updateHaze() {
    double unhazed = 1 - *pCoverHaze;
    for (auto screen :  screens)
        unhazed *= 1 - screen->pullValue<double>("state")*screen->pullValue<double>("haze");
    haze = 1 - unhazed;
}

void Shelter::updateAirTransmission() {
    airTransmission = 1;
    for (auto screen : screens)
        airTransmission *= 1 - screen->pullValue<double>("state")*(1 - screen->pullValue<double>("airTransmission"));
}

void Shelter::updateLightTransmission() {
    diffuseLightTransmission = *pCoverDiffuseTransmission;
    for (auto screen : screens)
        diffuseLightTransmission *= 1 - screen->pullValue<double>("state")*(1 - screen->pullValue<double>("transmission"));
    diffuseLightTransmission *= (1-chalk)*(1-greenhouseShade);

    double timeAndPlace = interpolate(*dirTransTable, latitude, azimuth);
    double directLightTransmission = diffuseLightTransmission*timeAndPlace;
    directLightTransmissionAsDirect = directLightTransmission*(1-haze);
    directLightTransmissionAsDiffuse = directLightTransmission*haze;
}

} //namespace

