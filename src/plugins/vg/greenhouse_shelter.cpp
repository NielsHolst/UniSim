/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include "greenhouse_shelter.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(GreenhouseShelter)

/*! \class GreenhouseShelter
 * \brief Average value of physical characteristics of all shelters
 *
 * Inputs
 * ------
 *
 * Output
 * ------
 * - _U_ is the average heat transfer coefficient of the surface (cover+screens) per ground area [W/K/m<SUP>2</SUP>]
 * - _Ucover_ is the average heat transfer coefficient of the surface (cover only) per ground area [W/K/m<SUP>2</SUP>]
 * - _airTransmission_ is the average proportion of air transmitted through the surface [0;1]
 * - _haze_ is the average proportion of direct light becoming dispersed on passage through the surface [0;1]
 * - _diffuseLightTransmission_ is the average proportion of diffuse light transmitted through the surface [0;1]
 * - _directLightTransmissionAsDirect_ is the average proportion of direct light transmitted through the surface as direct light [0;1]
 * - _directLightTransmissionAsDiffuse_ is the average proportion of direct light transmitted through the surface as diffuse light [0;1]
 * - _temperature_ is the average temperature of the surface [<SUP>o</SUP>C]
 *
 * Default dependencies
 * ------------
 */

GreenhouseShelter::GreenhouseShelter(Identifier name, QObject *parent)
    : ShelterOutputs(name, parent)
{
    InputRef(double, coverArea, "construction/geometry[coverArea]");
    InputRef(double, groundArea, "construction/geometry[groundArea]");
    Output(double, Ucover);
    Output(double, temperature);
}

void GreenhouseShelter::localReset() {
    shelterInfos.clear();
    for (auto shelter : seekChildren<Model*>("*")) {
        Model *cover = shelter->seekOneChild<Model*>("cover");
        double area = cover->pullValue<double>("area");
        const double *pTemperature = cover->seekOneChild<Model*>("energyFlux")->pullValuePtr<double>("temperature");
        const double *pUcover = cover->pullValuePtr<double>("U");
        shelterInfos << ShelterInfo{shelter, area, pTemperature, pUcover};
    }

    if (shelterInfos.size() != 6) {
        QString msg{"The number of greenhouse shelters (%1) must be 6"};
        throw Exception(msg.arg(shelterInfos.size()), this);
    }

    Ucover = 0.;
    temperature = 18.;
}

void GreenhouseShelter::update() {
    U = airTransmission = haze =
    diffuseLightTransmission = directLightTransmissionAsDirect = directLightTransmissionAsDiffuse =
    temperature = Ucover = 0;
    for (auto shelterInfo : shelterInfos) {
        U += shelterInfo.weightedShelterOutput("U");
        airTransmission += shelterInfo.weightedShelterOutput("airTransmission");
        haze += shelterInfo.weightedShelterOutput("haze");
        diffuseLightTransmission += shelterInfo.weightedShelterOutput("diffuseLightTransmission");
        directLightTransmissionAsDirect += shelterInfo.weightedShelterOutput("directLightTransmissionAsDirect");
        directLightTransmissionAsDiffuse += shelterInfo.weightedShelterOutput("directLightTransmissionAsDiffuse");
        temperature += *(shelterInfo.pTemperature)*shelterInfo.area;
        Ucover += *(shelterInfo.pUcover)*shelterInfo.area;
    }
    // Weighted averages
    U /= coverArea;
    airTransmission /= coverArea;
    haze /= coverArea;
    diffuseLightTransmission /= coverArea;
    directLightTransmissionAsDirect /= coverArea;
    directLightTransmissionAsDiffuse /= coverArea;
    temperature /= coverArea;
    Ucover /= coverArea;
    // From per m2 cover to per m2 ground area
    U *= groundArea/coverArea;
    Ucover *= groundArea/coverArea;
}


} //namespace

