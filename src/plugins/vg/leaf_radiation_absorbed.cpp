/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include <usbase/test_num.h>
#include "leaf_radiation_absorbed.h"
#include "general.h"
#include "publish.h"

using std::max;
using namespace UniSim;

namespace vg {

PUBLISH(LeafRadiationAbsorbed)


/*! \class LeafRadiationAbsorbed
 * \brief
 *
 * Inputs
 * ------

 * Output
 * ------
 *
 */

LeafRadiationAbsorbed::LeafRadiationAbsorbed(Identifier name, QObject *parent)
    : Model(name, parent)
{
    Input(double, kLw, 0.8);
    Input(double, emissivity, 0.8);
    InputRef(double, xGaussLowerside, "..[xGaussLowerside]");
    InputRef(double, wGaussLowerside, "..[wGaussLowerside]");
    InputRef(double, xGaussUpperside, "..[xGaussUpperside]");
    InputRef(double, wGaussUpperside, "..[wGaussUpperside]");
    InputRef(double, lai, "crop/lai[value]");
    InputRef(double, indoorsLight, "indoors/light[total]");
    InputRef(double, lightAbsorptivity, "../photosynthesis[absorptivity]");
    InputRef(double, heating, "heating/supply[value]");
//    InputRef(double, floorTemperature, "energyFlux/floor[temperature]");
    InputRef(double, growthLightLight, "actuators/growthlights[shortWaveEmission]");
    InputRef(double, growthLightLw, "actuators/growthlights[longWaveEmission]");
    Input(double, growthLightViewFactor, 1.);
    InputRef(double, leafTemperature, "../temperature[value]");
    InputRef(double, coverTemperature, "given/energyFlux/shelter[coverTemperature]");
    InputRef(double, screensTemperature, "given/energyFlux/shelter[screensTemperature]");
    InputRef(double, screensMaxState, "construction/shelters[screensMaxState]");
    InputRef(double, shelterOutgoingLwAbsorptivity, "construction/shelters[outgoingLwAbsorptivity]");
    InputRef(double, coverPerGroundArea, "construction/geometry[coverPerGroundArea]");

    Output(double, lightAbsorbed);
    Output(double, heatingAbsorbed);
    Output(double, growthLightLwAbsorbed);
    Output(double, shelterLoss);
    Output(double, value);
}

void LeafRadiationAbsorbed::initialize() {
    auto pipes = seekMany<Model*>("actuators/heating/pipes/*");
    for (Model *pipe : pipes) {
        pipeInfos << PipeInfo {
                        pipe->pullValuePtr<double>("length"),
                        pipe->pullValuePtr<double>("diameter"),
                        pipe->pullValuePtr<double>("temperature"),
                        pipe->pullValuePtr<double>("emissivity")
                     };
    }
}

void LeafRadiationAbsorbed::reset() {
    lightAbsorbed =
    heatingAbsorbed =
    growthLightLwAbsorbed =
    shelterLoss =
    value = 0.;
}

void LeafRadiationAbsorbed::update() {
    irTransmissionLowerside = kLw*exp(-kLw*lai*xGaussLowerside)*wGaussLowerside*lai;
    irTransmissionUpperside = kLw*exp(-kLw*lai*xGaussUpperside)*wGaussUpperside*lai;
    setLightAbsorbed();
    setGrowthLightLwAbsorbed();
    setShelterLoss();
    setHeatingAbsorbed();
    value = lightAbsorbed + heatingAbsorbed + growthLightLwAbsorbed - shelterLoss;
}

void LeafRadiationAbsorbed::setLightAbsorbed() {
    lightAbsorbed = lightAbsorptivity*(indoorsLight + growthLightLight);
}

void LeafRadiationAbsorbed::setGrowthLightLwAbsorbed() {
    // This is a shortcut. We should know the temperature and area of the lamps.
    growthLightLwAbsorbed = growthLightViewFactor*growthLightLw*irTransmissionUpperside*emissivity;
}

void LeafRadiationAbsorbed::setShelterLoss() {
    double
        em = jointEmissivity(emissivity, shelterOutgoingLwAbsorptivity),
        screensDiff = p4K(leafTemperature) - p4K(screensTemperature),
        coverDiff = p4K(leafTemperature) - p4K(coverTemperature);
    shelterLoss = Sigma*em*coverPerGroundArea*irTransmissionUpperside*
                  (screensDiff*screensMaxState + coverDiff*(1-screensMaxState));
}

void LeafRadiationAbsorbed::setHeatingAbsorbed() {
    heatingAbsorbed = 0;
    for (PipeInfo pi : pipeInfos) {
        if (*pi.temperature > leafTemperature) {
            double em = jointEmissivity(emissivity, *pi.emissivity);
            heatingAbsorbed += Sigma*em*(p4K(*pi.temperature) - p4K(leafTemperature))*irTransmissionLowerside*pi.area();
        }
    }
}


} //namespace

