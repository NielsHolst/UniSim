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
    Input(double, kIr, 0.8);
    Input(double, emissivity, 0.8);
    InputRef(double, xGaussLowerside, "..[xGaussLowerside]");
    InputRef(double, wGaussLowerside, "..[wGaussLowerside]");
    InputRef(double, xGaussUpperside, "..[xGaussUpperside]");
    InputRef(double, wGaussUpperside, "..[wGaussUpperside]");
    InputRef(double, lai, "crop/lai[value]");
    InputRef(double, indoorsLight, "indoors/light[total]");
    InputRef(double, lightAbsorptivity, "../photosynthesis[absorptivity]");
    InputRef(double, heating, "heating/supply[value]");
    InputRef(double, growthLightLight, "actuators/growthlights[shortWaveEmission]");
    InputRef(double, growthLightIr, "actuators/growthlights[longWaveEmission]");
    InputRef(double, leafTemperature, "../temperature[value]");
    InputRef(double, coverTemperature, "given/energyFlux/shelter[coverTemperature]");
    InputRef(double, screensTemperature, "given/energyFlux/shelter[screensTemperature]");
    InputRef(double, screensMaxState, "construction/shelters[screensMaxState]");
    InputRef(double, shelterOutgoingIrAbsorptivity, "construction/shelters[outgoingIrAbsorptivity]");
    InputRef(double, coverPerGroundArea, "construction/geometry[coverPerGroundArea]");

    Output(double, lightAbsorbed);
    Output(double, heatingAbsorbed);
    Output(double, growthLightIrAbsorbed);
    Output(double, shelterLoss);
    Output(double, value);
}
void LeafRadiationAbsorbed::reset() {
    lightAbsorbed =
    heatingAbsorbed =
    growthLightIrAbsorbed =
    shelterLoss =
    value = 0.;
}

void LeafRadiationAbsorbed::update() {
    double
//        irTransmissionLowerside = kIr*exp(-kIr*lai*xGaussLowerside)*wGaussLowerside*lai,
        irTransmissionUpperside = kIr*exp(-kIr*lai*xGaussUpperside)*wGaussUpperside*lai,
        emUpperside = jointEmissivity(shelterOutgoingIrAbsorptivity, emissivity),
        shelterTemperature = screensTemperature*screensMaxState + coverTemperature*(1-screensMaxState);

    lightAbsorbed = lightAbsorptivity*(indoorsLight + growthLightLight);
//    double heatingBalance = heating*irTransmissionLowerside - Sigma*p4K(leafTemperature);
//    heatingAbsorbed = max(emissivity*heatingBalance, 0.);
    /*  Heating absorbed removed. At a leaf temperature of e.g 17C, the heat emission from leaves is 400 W/m2.
        To heat the leaves above 17C by long-wave radiation, we would thus need a heat source yielding more than 400 W/m2.
        The heating of leaves must therefore be due to convection and conduction.
    */
//    growthLightIrAbsorbed = growthLightIr*irTransmissionUpperside*emissivity;
    /* Skipped for the same reason as above
     */
    shelterLoss = Sigma*emUpperside*(p4K(leafTemperature) - p4K(shelterTemperature))*coverPerGroundArea*irTransmissionUpperside;

    value = lightAbsorbed + heatingAbsorbed + growthLightIrAbsorbed - shelterLoss;
}

//void LeafRadiationAbsorbed::update() {
//    double
//        irAbsorptivityLowerside = kIr*exp(-kIr*lai*xGaussLowerside)*wGaussLowerside*lai,
//        irAbsorptivityUpperside = kIr*exp(-kIr*lai*xGaussUpperside)*wGaussUpperside*lai,
//        em = jointEmissivity(shelterOutgoingIrAbsorptivity, irAbsorptivityUpperside),
//        shelterTemperature = screensTemperature*screensMaxState + coverTemperature*(1-screensMaxState);

//    lightAbsorbed = lightAbsorptivity*(indoorsLight + growthLightLight);
//    heatingAbsorbed = heating*irAbsorptivityLowerside;
//    growthLightIrAbsorbed = growthLightIr*irAbsorptivityUpperside;
//    shelterLoss = Sigma*em*(p4K(leafTemperature) - p4K(shelterTemperature))*coverPerGroundArea;

//    value = lightAbsorbed + heatingAbsorbed + growthLightIrAbsorbed - shelterLoss;
//}

} //namespace

