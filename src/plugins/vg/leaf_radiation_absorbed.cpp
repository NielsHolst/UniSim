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
    InputRef(double, xGaussLowerside, "..[xGaussLowerside]");
    InputRef(double, wGaussLowerside, "..[wGaussLowerside]");
    InputRef(double, xGaussUpperside, "..[xGaussUpperside]");
    InputRef(double, wGaussUpperside, "..[wGaussUpperside]");
    InputRef(double, lai, "crop/lai[lai]");
    InputRef(double, indoorsLight, "indoors/light[total]");
    InputRef(double, lightAbsorptivity, "../photosynthesis[absorptivity]");
    InputRef(double, heating, "heating/supply[value]");
    InputRef(double, growthLightLight, "actuators/growthlights[shortWaveEmission]");
    InputRef(double, growthLightIr, "actuators/growthlights[longWaveEmission]");
    InputRef(double, temperature, "../temperature[value]");

    Output(double, lightAbsorbed);
    Output(double, heatingAbsorbed);
    Output(double, growthLightIrAbsorbed);
    Output(double, coverLoss);
    Output(double, value);
}

void LeafRadiationAbsorbed::initialize() {
    auto shelters = seekMany<Model*>("construction/shelters/*");
    for (Model *shelter : shelters) {
        Model *cover = shelter->seekOneChild<Model*>("cover"),
              *temperature = cover->seekOneChild<Model*>("energyFlux");
        shelterInfos <<
            ShelterInfo {
                shelter->pullValuePtr<double>("areaPerGround"),
                shelter->pullValuePtr<double>("maxScreenState"),
                temperature->pullValuePtr<double>("temperature"),
                cover->pullValuePtr<double>("outgoingIrAbsorptivity")
            };
    }
}

void LeafRadiationAbsorbed::reset() {
    lightAbsorbed =
    heatingAbsorbed =
    growthLightIrAbsorbed =
    coverLoss =
    value = 0.;
}

void LeafRadiationAbsorbed::update() {
    irAbsorptivityLowerside = kIr*exp(-kIr*lai*xGaussLowerside)*wGaussLowerside*lai,
    irAbsorptivityUpperside = kIr*exp(-kIr*lai*xGaussUpperside)*wGaussUpperside*lai;
    lightAbsorbed = lightAbsorptivity*(indoorsLight + growthLightLight);
    heatingAbsorbed = heating*irAbsorptivityLowerside;
    growthLightIrAbsorbed = growthLightIr*irAbsorptivityUpperside;
    computeCoverLoss();
    value = lightAbsorbed + heatingAbsorbed + growthLightIrAbsorbed - coverLoss;
}

void LeafRadiationAbsorbed::computeCoverLoss() {
    coverLoss = 0.;
    for (ShelterInfo si : shelterInfos) {
        double em = 1/(1/irAbsorptivityUpperside + 1/(*si.emissivity) - 1);
        coverLoss += Sigma*em*(p4K(temperature) - p4K(*si.temperature))*(*si.areaPerGround)*(1-(*si.maxState));
    }
}

} //namespace

