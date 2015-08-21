/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include <usbase/decode_list.h>
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
 * - _shelters_ is a list of the shelters over which to average [string-list]
 * - _groundArea_ is the area covered by the greenhouse [m<SUP>2</SUP>]
 *
 * Output
 * ------
 * - _U_ is the average heat transfer coefficient of the surface (cover+screens) per ground area [W/K/m<SUP>2</SUP>]
 * - _Ucover_ is the average heat transfer coefficient of the surface (cover only) per ground area [W/K/m<SUP>2</SUP>]
 * - _airTransmissivity_ is the average proportion of air transmitted through the surface [0;1]
 * - _haze_ is the average proportion of direct light becoming dispersed on passage through the surface [0;1]
 * - _diffuseLightTransmission_ is the average proportion of diffuse light transmitted through the surface [0;1]
 * - _directLightTransmissionAsDirect_ is the average proportion of direct light transmitted through the surface as direct light [0;1]
 * - _directLightTransmissionAsDiffuse_ is the average proportion of direct light transmitted through the surface as diffuse light [0;1]
 * - _temperature_ is the average temperature of the surface [<SUP>o</SUP>C]
 */

GreenhouseShelter::GreenhouseShelter(Identifier name, QObject *parent)
    : ShelterOutputs(name, parent)
{
    Input(QString, shelters, "()");
    InputRef(double, groundArea, "construction/geometry[groundArea]");
    Output(double, Ucover);
    Output(double, temperature);
}

void GreenhouseShelter::localReset() {
    QStringList modelNames = decodeList(shelters, this);
    QVector<Model*> shelterModels;
    for (auto modelName : modelNames) {
        auto model = seekOne<Model*>(modelName);
        shelterModels << model;
    }

    shelterInfos.clear();
    totalArea = 0.;
    for (Model *shelter : shelterModels) {
        Model *cover = shelter->seekOneChild<Model*>("cover");
        double area = cover->pullValue<double>("area");
        const double *pTemperature = cover->seekOneChild<Model*>("energyFlux")->pullValuePtr<double>("temperature");
        const double *pUcover = cover->pullValuePtr<double>("U");
        shelterInfos << ShelterInfo{shelter, area, pTemperature, pUcover};
        totalArea += area;
    }

    Ucover = 0.;
    temperature = 18.;
}

void GreenhouseShelter::update() {
    U = airTransmissivity = haze =
    diffuseLightTransmission = directLightTransmissionAsDirect = directLightTransmissionAsDiffuse =
    temperature = Ucover = 0;
    for (auto shelterInfo : shelterInfos) {
        U += shelterInfo.weightedShelterOutput("U");
        airTransmissivity += shelterInfo.weightedShelterOutput("airTransmissivity");
        haze += shelterInfo.weightedShelterOutput("haze");
        diffuseLightTransmission += shelterInfo.weightedShelterOutput("diffuseLightTransmission");
        directLightTransmissionAsDirect += shelterInfo.weightedShelterOutput("directLightTransmissionAsDirect");
        directLightTransmissionAsDiffuse += shelterInfo.weightedShelterOutput("directLightTransmissionAsDiffuse");
        temperature += *(shelterInfo.pTemperature)*shelterInfo.area;
        Ucover += *(shelterInfo.pUcover)*shelterInfo.area;
    }
    // Weighted averages
    U /= totalArea;
    airTransmissivity /= totalArea;
    haze /= totalArea;
    diffuseLightTransmission /= totalArea;
    directLightTransmissionAsDirect /= totalArea;
    directLightTransmissionAsDiffuse /= totalArea;
    temperature /= totalArea;
    Ucover /= totalArea;
    // From per m2 cover to per m2 ground area
    U *= groundArea/totalArea;
    Ucover *= groundArea/totalArea;
}


} //namespace

