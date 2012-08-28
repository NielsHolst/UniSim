/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include <QtAlgorithms>
#include <usbase/exception.h>
#include <usbase/test_num.h>
#include "../unisim/stage.h"
#include "area.h"
#include "mass.h"
#include "organ.h"
#include "plant.h"

using namespace UniSim;

namespace intercom{

Plant::Plant(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("density", &density, 100., this,
                          "Plants per m @Sup {2}");

    new Variable<double>("mass", &mass, this,
                             "Mass of one plant (g per plant)");
    new Variable<double>("LAI", &lai, this,
                             "Leaf area index of whole plant (m @Sup 2 plant area per m @Sup 2 ground)");
    new Variable<double>("lightAbsorption", &lightAbsorption, this,
                             "Absorbed light (W per m @Sup 2 ground per day)");
    new Variable<double>("allocatedPerPlant", &allocatedPerPlant, this,
                             "Net allocation rate, the daily increment to plant @F mass (g per plant per day)");

    new Variable<double>("CO2Assimilation", &CO2Assimilation, this,
                             "Assimilated CO @Sub 2 (kg CO @Sub 2 per ha ground per day");
    new Variable<double>("grossProduction", &grossProduction, this,
                             "Produced kg CH @Sub {2}O per ha ground per day");
    new Variable<double>("maintenanceResp", &maintenanceResp, this,
                             "Maintenance respiration (kg CH @Sub {2}O per ha ground per day)");
    new Variable<double>("growthResp", &growthResp, this,
                             "Growth respiration (kg CH @Sub 2 O per ha ground per day)");
    new Variable<double>("availableProduction", &availableProduction, this,
                             "The available production is gross production minus maintenance respiration "
                             "(kg CH @Sub {2}O per ha ground per day)");
    new Variable<double>("netAllocation", &netAllocation, this,
                             "Net allocation rate, the daily increment to plant population @F mass "
                             "(kg CH @Sub {2}O per ha ground per day)");
}

void Plant::initialize() {
    earlyGrowth = seekOneChild<Model*>("earlyGrowth");

    QList<Organ*> organModels = seekChildren<Organ*>("*");
    for (int i = 0; i < organModels.size(); ++i) {
        OrganInfo info = {0,0,0,0,0,0,0};
        info.organ = organModels[i];
        info.mass = info.organ->seekOneChild<Mass*>("*");
        info.area = info.organ->peekOneChild<Area*>("*");
        info.massValue = info.mass->pullValuePtr<double>("value");

        Model *partion = info.mass->seekOneChild<Model*>("partition");
        info.idealPartitioning = partion->pullValuePtr<double>("value");

        if (info.area) {
            Model *sla = info.area->seekOneChild<Model*>("specificLeafArea");
            info.sla = sla->pullValuePtr<double>("value");
        }
        else {
            info.sla = 0;
        }

        info.carbohydrateRequirements = info.organ->pullValue<double>("CH2ORequirement");

        organs.append(info);
        solution.append(OptimizeAllocation::SolutionEntry());
    }

    Model *phenology = seekOneChild<Model*>("phenology");
    QList<Stage*> stages = phenology->seekChildren<Stage*>("*");
}

void Plant::reset() {
    lightAbsorption =
    CO2Assimilation =
    grossProduction =
    maintenanceResp =
    availableProduction =
    netAllocation =
    earlyGrowthMass.previous = 0;
    updatePullVariables();
}

void Plant::updatePullVariables() {
    lai = 0.;
    for (int i = 0; i < organs.size(); ++i)
        if (organs[i].area)
            lai += organs[i].area->pullValue<double>("lai");

    mass = 0;
    for (int i = 0; i < organs.size(); ++i)
        mass += *organs[i].massValue;

    for (int i = 0; i < organs.size(); ++i)
        organs[i].mass->updateCurrentPartition(mass);    
}

void Plant::updateEarlyGrowth() {
    double idealWeightedSum = 0.;
    double idealSumAreas = 0;
    for (int i = 0; i < organs.size(); ++i) {
        if (organs[i].area) {
            double ideal = *organs[i].idealPartitioning;
            idealWeightedSum += ideal* *organs[i].sla;
            idealSumAreas += ideal;
        }
    }

    double totalArea = earlyGrowth->pullValue<double>("value");
    double totalAreaMass = (idealWeightedSum == 0) ? 0. : totalArea/idealWeightedSum;

    double increment;
    earlyGrowthMass.previous = earlyGrowthMass.current;
    if (idealSumAreas == 0.) {
        earlyGrowthMass.current =
        increment = 0.;
    }
    else {
        earlyGrowthMass.current = totalAreaMass/idealSumAreas;
        increment = earlyGrowthMass.current - earlyGrowthMass.previous;
        TestNum::assureGeZero(increment, "early growth increment", this);
    }

    allocatedPerPlant =
    netAllocation =
    growthResp = 0.;
    for (int i = 0; i < organs.size(); ++i) {
        double ideal = *organs[i].idealPartitioning;
        Organ *organ = organs[i].organ;
        organ->allocateNet(ideal, increment);
        allocatedPerPlant += organ->pullValue<double>("allocatedPerPlant");
        netAllocation += organ->pullValue<double>("netAllocation");
        growthResp += organ->pullValue<double>("growthResp");
    }
    updatePullVariables();
}


void Plant::updatePhotosynthesis() {
    lightAbsorption =
    CO2Assimilation =
    grossProduction =
    maintenanceResp = 0.;
    for (int i = 0; i < organs.size(); ++i) {
        Organ *organ = organs[i].organ;
        organ->updatePhotosynthesis();
        lightAbsorption += organ->pullValue<double>("lightAbsorption");
        CO2Assimilation += organ->pullValue<double>("CO2Assimilation");
        grossProduction += organ->pullValue<double>("grossProduction");
        maintenanceResp += organ->pullValue<double>("maintenanceResp");
    }
    allocate();
}

void Plant::allocate() {
    availableProduction = grossProduction - maintenanceResp;
    if (availableProduction < 0.)
        availableProduction = 0.;

    calcOptimalPartioning();

    allocatedPerPlant =
    netAllocation =
    growthResp = 0.;
    for (int i = 0; i < organs.size(); ++i) {
        Organ *organ = organs[solution[i].second].organ;
        double proportion = solution[i].first;
        organ->allocate(proportion, availableProduction);
        allocatedPerPlant += organ->pullValue<double>("allocatedPerPlant");
        netAllocation += organ->pullValue<double>("netAllocation");
        growthResp += organ->pullValue<double>("growthResp");
    }
    updatePullVariables();
}

void Plant::calcOptimalPartioning() {
    double M = 0.;
    int n = organs.size();
    for (int i = 0; i < n; ++i)
        M += *organs.at(i).massValue;

    double sumIr = 0;
    for (int i = 0; i < n; ++i) {
        double mass = *organs.at(i).massValue;
        double idealPart = *organs.at(i).idealPartitioning;
        double req = organs.at(i).carbohydrateRequirements;
        double optimalPart =
            (availableProduction == 0.) ? idealPart : (idealPart*(M + req*availableProduction) - mass)/availableProduction;

        solution[i] = OptimizeAllocation::SolutionEntry(optimalPart, i);
        sumIr += idealPart*req;
    }
    if (availableProduction == 0.)
        return;

    Q_ASSERT(sumIr>0);
    for (int i = 0; i < n; ++i) {
        solution[i].first /= sumIr;
    }

    OptimizeAllocation::optimize(solution);

    double checkSum = 0;
    for (int i = 0; i < n; ++i)
        checkSum += solution.at(i).first;
    TestNum::assureEq(checkSum, 1., "Sum of partitions", this);
}

double Plant::kgPerHa_to_gPerPlant(double x) const{
    return x             // kg/ha
           *1000./10000. // g/m2
           /density;     // g per plant

}

double Plant::gPerPlant_to_kgPerHa(double x) const{
    return x
           /1000.*10000.
           *density;
}

} //namespace

