/* Copyright (C)reproduction 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include <usbase/exception.h>
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include <usbase/test_num.h>
#include "acquisition.h"
#include "holometabola.h"
#include "life_stage.h"

using namespace UniSim;


namespace SupplyDemand {

Acquisition::Acquisition(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<QString>("resource", &resourceStr, QString("resource"), this, "Name of resource model");
    new Parameter<double>("attackRate", &attackRate, 1., this, "Attack rate (per capita)");

    new Variable<double>("egestion", &a.egestion, this, "Egestion (mass) in this time step");
    new Variable<double>("respiration", &a.respiration, this, "Allocation to respiration (mass) in this time step");
    new Variable<double>("reproduction", &a.reproduction, this, "Allocation to reproduction (mass) in this time step");
    new Variable<double>("growth", &a.growth, this, "Allocation to growth (mass) in this time step");
    new Variable<double>("sdReproduction", &a.sdReproduction, this, "Supply/Demand ratio for reproduction this time step");
    new Variable<double>("sdGrowth", &a.sdGrowth, this, "Supply/Demand ratio for growth this time step");
    new Variable<double>("deficit", &a.deficit, this, "Energy budget deficit (positive, mass) in this time step. "
                             "Arises if respiration demand exceeds the acquired supply");
}

void Acquisition::initialize() {
    Model *resourceModel = seekOne<Model*>(resourceStr);
    resource = resourceModel->pullValuePtr<double>("value");

    d.pEgestion = lookupDemand("egestion");
    d.pRespiration = lookupDemand("respiration");
    d.pReproduction = lookupDemand("reproduction");
    d.pGrowth = lookupDemand("growth");

    Model *reproduction = peekOneSibling<Model*>("reproduction");
    eggWeight = reproduction ? reproduction->pullValue<double>("eggWeight") : 0.;

    Model *growth = peekOneSibling<Model*>("growth");
    maxGrowthRate = growth ? growth->pullValuePtr<double>("maxGrowthRate") : 0;

    LifeStage *stage = seekNearestAscendant<LifeStage*>("*");
    myMassModel = stage->peekOneDescendant<Model*>("lifetable/mass");
    myMass = myMassModel->pullValuePtr<double>("value");
    myDuration = myMassModel->pullValuePtr<double>("duration");

    Holometabola *insect = seekOneAscendant<Holometabola*>("*");
    eggMass = insect->peekOneDescendant<Model*>("egg/lifetable/mass");
    eggNumber = insect->peekOneDescendant<Model*>("egg/lifetable/number");
}

const double* Acquisition::lookupDemand(QString demandName) {
    Model *demandModel = peekOneSibling<Model*>(demandName);
    return demandModel ? demandModel->pullValuePtr<double>("value") : 0;
}

void Acquisition::reset() {
    a.setToZero();
}

void Acquisition::update() {
    demandTotal = d.egestion() < 1. ? (d.respiration() + d.reproduction() + d.growth())/(1. - d.egestion()) : 0;
    supplyTotal = GBFuncResp(demandTotal, attackRate*(*resource));
    double available = supplyTotal;

    a.setToZero();
    a.egestion = supplyTotal*d.egestion();
    a.respiration = d.respiration();
    available -= a.egestion + a.respiration;
    if (available <= 0) {
        a.deficit = -available;
        return;
    }
    allocate(d.reproduction(), available, a.reproduction, a.sdReproduction);
    allocate(d.growth(), available, a.growth, a.sdGrowth);
    Q_ASSERT(TestNum::eqZero(available));

    if (eggMass)
        eggMass->pushValue<double>("inflow", a.reproduction);
    if (eggNumber && eggWeight > 0.)
        eggNumber->pushValue<double>("inflow", a.reproduction/eggWeight);
    if (myMassModel && maxGrowthRate) {
        Q_ASSERT(*maxGrowthRate > 0.);
        myMassModel->pushValue<double>("growthRate", pow(*maxGrowthRate, a.sdGrowth));
    }
}

void Acquisition::allocate(double demand, double &available, double &allocation, double &sd) {
    allocation = demand <= available ? demand : available;
    available -= allocation;
    sd = demand > 0 ? allocation/demand : 0.;
    TestNum::snapTo(sd, 1.);
    Q_ASSERT(demand>=0. && available>=0. && allocation>=0. && sd>=0. && sd<=1.);
}

} //namespace

