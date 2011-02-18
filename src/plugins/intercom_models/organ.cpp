/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include <usbase/exception.h>
#include <usbase/pull_variable.h>
#include <usbase/parameter.h>
#include <usbase/test_num.h>
#include "area.h"
#include "organ.h"
#include "plant.h"

using namespace UniSim;

namespace intercom{

Organ::Organ(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("maintenanceCoeff", &maintenanceCoeff, 0.03, this,
                          "Maintenance coefficient \"(g/g/day)\" at 20 {@Degree}C");
    new Parameter<double>("CH2ORequirement", &CH2ORequirement, 1.5, this,
                          "Carbohydrate requirement g CH @Sub 2 O per g biomass");

    new PullVariable<double>("lightAbsorption", &lightAbsorption, this,
                             "Absorbed light (W per m @Sup 2 ground per day)");
    new PullVariable<double>("CO2Assimilation", &CO2Assimilation, this,
                             "Assimilated kg CO @Sub 2 per ha ground per day");
    new PullVariable<double>("maintenanceResp", &maintenanceResp, this,
                             "Maintenance respiration (kg CH @Sub 2 O per ha ground per day)");
    new PullVariable<double>("growthResp", &growthResp, this,
                             "Growth respiration (kg CH @Sub 2 O per ha ground per day)");
}

void Organ::initialize() {
    if (TestNum::lt(CH2ORequirement, 1.))
        throw Exception("Parameter CH2ORequirement must be >= 1");
    weather = seekOne<Model*>("weather");
    plant = seekOneAscendant<Plant*>("*");
    partition = seekOneChild<Model*>("partition");
    mass = seekOneChild<Model*>("mass");
    area = peekOneChild<Model*>("area");
}

void Organ::reset() {
    lightAbsorption =
    CO2Assimilation =
    maintenanceResp =
    growthResp = 0.;
}

void Organ::accumulate() {
    updateMaintenanceRespiration();

    if (area) {
        lightAbsorption = area->pullVariable<double>("lightAbsorption");
        CO2Assimilation = area->pullVariable<double>("CO2Assimilation");;
    }
}

void Organ::updateMaintenanceRespiration() {
    double temp = weather->pullVariable<double>("Tavg");
    double resp20 = maintenanceCoeff*mass->pullVariable<double>("number");
    resp20 = plant->gPerPlant_to_kgPerHa(resp20);
    maintenanceResp = resp20*pow(2., (temp - 20.)/10.);
}

void Organ::setMass(double newMass) {
    double curMass = mass->pullVariable<double>("number");
    double increment = newMass - curMass;
    if (increment < 0.) increment = 0;
    addMass(increment);
}

void Organ::addMass(double increment) {
    mass->pushVariable<double>("inflow", increment);
    if (area)
        area->pushVariable<double>("allocation", increment);
}

double Organ::allocate(double carbohydrates) {
    double partioningCoeff = partition->pullVariable<double>("value");
    double allocatedCH2O = partioningCoeff*carbohydrates;
    double allocatedBiomass = allocatedCH2O/CH2ORequirement;
    growthResp = allocatedCH2O - allocatedBiomass;
    double allocatedPerPlant = plant->kgPerHa_to_gPerPlant(allocatedBiomass);
    addMass(allocatedPerPlant);
    return allocatedCH2O;
}

} //namespacee

