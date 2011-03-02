/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include <QtAlgorithms>
#include <usbase/exception.h>
#include <usbase/test_num.h>
#include "../unisim_models/stage.h"
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

    new PullVariable<double>("density", &density, this,
                             "Plants per m @Sup {2}");
    new PullVariable<double>("mass", &mass, this,
                             "Mass of one plant (g per plant)");
    new PullVariable<double>("netAllocation", &netAllocation, this,
                             "Net allocation rate, the daily increment to plant @F mass (g per plant per day)");
    new PullVariable<double>("LAI", &lai, this,
                             "Leaf area index of whole plant (m @Sup 2 plant area per m @Sup 2 ground)");
    new PullVariable<double>("lightAbsorption", &lightAbsorption, this,
                             "Absorbed light (W per m @Sup 2 ground per day)");
    new PullVariable<double>("CO2Assimilation", &CO2Assimilation, this,
                             "Assimilated CO @Sub 2 (kg CO @Sub 2 per ha ground per day");
    new PullVariable<double>("grossProduction", &grossProduction, this,
                             "Produced kg CH @Sub {2}O per ha ground per day");
    new PullVariable<double>("maintenanceResp", &maintenanceResp, this,
                             "Maintenance respiration (kg CH @Sub {2}O per ha ground per day)");
    new PullVariable<double>("availableProduction", &availableProduction, this,
                             "The available production is gross production minus maintenance respiration "
                             "(kg CH @Sub {2}O per ha ground per day");
}

void Plant::initialize() {
    earlyGrowth = seekOneChild<Model*>("earlyGrowth");

    QList<Organ*> organModels = seekChildren<Organ*>("*");
    for (int i = 0; i < organModels.size(); ++i) {
        OrganInfo info = {0,0,0,0,0,0,0};
        info.organ = organModels[i];
        info.mass = info.organ->seekOneChild<Mass*>("*");
        info.area = info.organ->peekOneChild<Area*>("*");
        info.massValue = info.mass->pullVariablePtr<double>("value");

        Model *partion = info.mass->seekOneChild<Model*>("partition");
        info.idealPartitioning = partion->pullVariablePtr<double>("value");

        if (info.area) {
            Model *sla = info.area->seekOneChild<Model*>("specificLeafArea");
            info.sla = sla->pullVariablePtr<double>("value");
        }
        else {
            info.sla = 0;
        }

        info.carbohydrateRequirements = info.organ->parameter<double>("CH2ORequirement");

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
            lai += organs[i].area->pullVariable<double>("lai");

    mass = 0;
    for (int i = 0; i < organs.size(); ++i)
        mass += *organs[i].massValue;

    for (int i = 0; i < organs.size(); ++i)
        organs[i].mass->updateCurrentPartition(mass);    
}

void Plant::applyEarlyGrowth() {
    double idealWeightedSum = 0.;
    double idealSumAreas = 0;
    for (int i = 0; i < organs.size(); ++i) {
        if (organs[i].area) {
            double ideal = *organs[i].idealPartitioning;
            idealWeightedSum += ideal* *organs[i].sla;
            idealSumAreas += ideal;
        }
    }

    double totalArea = earlyGrowth->pullVariable<double>("area");
    double totalAreaMass = (idealWeightedSum == 0) ? 0. : totalArea/idealWeightedSum;

    earlyGrowthMass.previous = earlyGrowthMass.current;
    earlyGrowthMass.current = (idealSumAreas == 0.) ? 0. : totalAreaMass/idealSumAreas;
    double increment = earlyGrowthMass.current - earlyGrowthMass.previous;
    TestNum::assureGeZero(increment, "early growth increment", this);

    netAllocation = 0.;
    for (int i = 0; i < organs.size(); ++i) {
        double ideal = *organs[i].idealPartitioning;
        netAllocation += organs[i].organ->allocateNet(ideal, increment);
    }
    updatePullVariables();
}

void Plant::accumulate() {
    lightAbsorption =
    CO2Assimilation =
    maintenanceResp = 0.;
    for (int i = 0; i < organs.size(); ++i) {
        organs[i].organ->accumulate();
        lightAbsorption += organs[i].organ->pullVariable<double>("lightAbsorption");
        CO2Assimilation += organs[i].organ->pullVariable<double>("CO2Assimilation");
        maintenanceResp += organs[i].organ->pullVariable<double>("maintenanceResp");
    }
}

void Plant::allocate(double carbohydrates) {
    grossProduction = carbohydrates;
    availableProduction = grossProduction - maintenanceResp;
    if (availableProduction < 0.)
        availableProduction = 0.;
    calcOptimalPartioning();

    netAllocation = 0.;
    for (int i = 0; i < organs.size(); ++i) {
        Organ *organ = organs[solution[i].second].organ;
        double proportion = solution[i].first;
        netAllocation += organ->allocate(proportion, availableProduction);
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

