/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include <usbase/exception.h>
#include <usbase/variable.h>
#include <usbase/parameter.h>
#include <usbase/test_num.h>
#include "area.h"
#include "community.h"
#include "organ.h"
#include "plant.h"

using namespace UniSim;

namespace intercom{

Organ::Organ(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("maintenanceCoeff", &maintenanceCoeff, 0.03, this,
                          "Maintenance coefficient (g/g/day) at 20 {@Degree}C");
    new Parameter<double>("CH2ORequirement", &CH2ORequirement, 1.5, this,
                          "Carbohydrate requirement g CH @Sub 2 O per g biomass");

    new Variable<double>("lightAbsorption", &lightAbsorption, this,
                             "Absorbed light (W per m @Sup 2 ground per day)");
    new Variable<double>("CO2Assimilation", &CO2Assimilation, this,
                             "Assimilated kg CO @Sub 2 per ha ground per day");
    new Variable<double>("grossProduction", &grossProduction, this,
                             "Produced kg CH @Sub {2}O per ha ground per day");
    new Variable<double>("maintenanceResp", &maintenanceResp, this,
                             "Maintenance respiration (kg CH @Sub 2 O per ha ground per day)");
    new Variable<double>("growthResp", &growthResp, this,
                             "Growth respiration (kg CH @Sub 2 O per ha ground per day)");
    new Variable<double>("netAllocation", &netAllocation, this,
                             "Net allocation rate, the daily increment to organbiomass (kg CH @Sub 2 O per ha ground per day)");
    new Variable<double>("allocatedPerPlant", &allocatedPerPlant, this,
                             "Net biomass allocated to this organ per plant (g per plant per day)");
    new Variable<double>("propAllocatedPerPlant", &propAllocatedPerPlant, this,
                             "Proportion biomass allocated to this organ among all the plant's organs [0;1]");
}

void Organ::initialize() {
    if (TestNum::lt(CH2ORequirement, 1.))
        throw Exception("Parameter CH2ORequirement must be >= 1");
    weather = seekOne<Model*>("weather");
    community = seekOneAscendant<Community*>("*");
    plant = seekOneAscendant<Plant*>("*");
    mass = seekOneChild<Model*>("mass");
    area = peekOneChild<Model*>("area");
}

void Organ::reset() {
    lightAbsorption =
    CO2Assimilation =
    maintenanceResp =
    grossProduction =
    growthResp = 0.;
    isTestMode = community->pullValue<bool>("testMode");
}

void Organ::updatePhotosynthesis() {
    if (area) {
        lightAbsorption = area->pullValue<double>("lightAbsorption");
        CO2Assimilation = area->pullValue<double>("CO2Assimilation");;
        grossProduction = area->pullValue<double>("grossProduction");;
    }
    updateMaintenanceRespiration();
}

void Organ::updateMaintenanceRespiration() {
    double temp = weather->pullValue<double>("Tavg");
    double resp20 = maintenanceCoeff*mass->pullValue<double>("value");
    resp20 = plant->gPerPlant_to_kgPerHa(resp20);
    maintenanceResp = resp20*pow(2., (temp - 20.)/10.);
}

void Organ::allocate(double proportion, double totalCarbohydrates) {
    propAllocatedPerPlant = proportion;
    double myShare = proportion*totalCarbohydrates;
    double netCarbohydrates = myShare/CH2ORequirement;
    doAllocate(myShare, netCarbohydrates);
}

void Organ::allocateNet(double proportion, double netCarbohydrates) {
    propAllocatedPerPlant = proportion;
    double myShare = proportion*netCarbohydrates;
    double totalCarbohydrates = myShare*CH2ORequirement;
    doAllocate(totalCarbohydrates, myShare);
}

void Organ::doAllocate(double totalCarbohydrates, double netCarbohydrates) {
    TestNum::assureGeZero(totalCarbohydrates, "totalCarbohydrates", this);
    TestNum::assureGeZero(netCarbohydrates, "netCarbohydrates", this);

    growthResp = totalCarbohydrates - netCarbohydrates;
    netAllocation = netCarbohydrates;
    allocatedPerPlant = plant->kgPerHa_to_gPerPlant(netCarbohydrates);

    bool isEarly = community->pullValue<bool>("isInEarlyGrowth");
    bool skipAllocation = isTestMode && !isEarly;
    if (!skipAllocation) {
        mass->pushValue<double>("allocation", allocatedPerPlant);
        if (area)
            area->pushValue<double>("allocation", allocatedPerPlant);
    }
}


} //namespacee

