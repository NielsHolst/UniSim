/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include <usbase/exception.h>
#include <usbase/test_num.h>
#include "area.h"
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
    organs = seekChildren<Organ*>("*");
    areas = seekDescendants<Area*>("*");
    for (int i = 0; i < areas.size(); ++i) {
        Model *sla = areas[i]->seekOneChild<Model*>("specificLeafArea");
        Model *partion = areas[i]->seekOneNearest<Model*>("partition");
        specificLeafAreas.append(sla);
        partitions.append(partion);
    }
}

void Plant::reset() {
    lightAbsorption =
    CO2Assimilation =
    grossProduction =
    maintenanceResp =
    availableProduction = 0.;
}

void Plant::updateLai() {
    lai = 0.;
    for (int i = 0; i < areas.size(); ++i)
        lai += areas[i]->pullVariable<double>("lai");
}

void Plant::applyEarlyGrowth() {
    double weights = 0.;
    for (int i = 0; i < areas.size(); ++i) {
        double sla = specificLeafAreas[i]->pullVariable<double>("value");
        double partition = partitions[i]->pullVariable<double>("value");
        weights += partition*sla;
    }

    double totalArea = earlyGrowth->pullVariable<double>("area");
    double totalMass = (weights == 0) ? 0. : totalArea/weights;

    for (int i = 0; i < areas.size(); ++i) {
        double partition = partitions[i]->pullVariable<double>("value");
        double mass = partition*totalMass;
        organs[i]->setMass(mass);
    }
    updateLai();
}

void Plant::accumulate() {
    reset();
    for (int i = 0; i < organs.size(); ++i) {
        organs[i]->accumulate();
        lightAbsorption += organs[i]->pullVariable<double>("lightAbsorption");
        CO2Assimilation += organs[i]->pullVariable<double>("CO2Assimilation");
        maintenanceResp += organs[i]->pullVariable<double>("maintenanceResp");
    }
}

void Plant::allocate(double carbohydrates) {
    grossProduction = carbohydrates;
    availableProduction = grossProduction - maintenanceResp;
    if (availableProduction < 0.)
        availableProduction = 0.;

    double allocated = 0.;
    for (int i = 0; i < organs.size(); ++i) {
        allocated += organs[i]->allocate(availableProduction);
    }
    if (TestNum::ne(allocated, availableProduction))
        throw Exception("Carbohydrate allocation differs from supply: " +
                        QString::number(allocated) + " != " +
                        QString::number(availableProduction), this);

    updateLai();
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

