/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <cmath>
#include <QMessageBox>
#include <QString>
#include <QTextStream>
#include <usbase/test_num.h>
#include <usbase/exception.h>
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include <usbase/test_num.h>
#include <usbase/utilities.h>
#include "stage.h"
	
namespace UniSim {

Stage::Stage(UniSim::Identifier name, QObject *parent)
    : UniSim::Model(name, parent), dd(0)
{
    new Parameter<int>("k", &k, 30, this,
        "The number of age classes in the stage. The fewer age classes, the larger the variance on @F {duration}");
    new Parameter<double>("duration", &L, 100., this,
        "The average duration of the stage: an inflow will emerge as an outflow dispersed "
        "over time, with a delay of @F duration on average and a variance of @Math {@F duration sup 2 slash @F k sup 2} "
        "@Cite{$label{(Manetsch 1976)}manetsch}");
    new Parameter<double>("growthRate", &fgr, 1., this,
        "For every quantity @I x that enters as inflow, @I growthRate*x will emerge as outflow. "
        "@F growthRate can be changed during the simulation but must be larger than zero. "
        "Use small values (e.g., @Math {10 sup {-6}}) instead of zero. "
        "@F growthRate is also a pull/push variable");
    new Parameter<double>("initialInflow", &initialInflow, 0., this,
        "The @F initialInflow is entered as inflow at time 0");
    new Parameter<double>("inflow", &inflow, 0., this,
        "Number of units to be put into the stage in the next time step");
    new Parameter<double>("sdRatio", &sdRatio, 1., this,
        "Supply/Demand ratio");
    new Parameter<double>("instantMortality", &instantMortality, 0., this,
        "Mortality [0..100] will be applied in the next time step, before @F inflow is added");

    new Variable<double>("value", &sum, this,
        "Number of units (e.g. individuals) in stage");
    new Variable<double>("number", &sum, this,
        "Synonymous with @F {value}");
    new Variable<double>("latestInflow", &latestInflow, this,
        "Inflow into the stage in latest time step");
    new Variable<double>("outflow", &outflow, this,
        "Outflow from the stage in latest time step");
    new Variable<double>("inflowTotal", &inflowTotal, this,
        "Total inflow into the stage since beginning of the simulation");
    new Variable<double>("outflowTotal", &outflowTotal, this,
        "Total outflow from the stage since beginning of the simulation");
    new Variable<double>("timeStep", &dt, this,
        "The latest time step applied to the stage");
    new Variable<double>("growthIncrement", &growth, this,
        "Increment realised in this integration step");
}

Stage::~Stage() {
    delete dd;
}

void Stage::initialize()
{
    if (k <= 0)
        throw Exception(QString("k (%1) must be > 0").arg(k), this);
    if (L <= 0)
        throw Exception(QString("Duration (%1) must be > 0").arg(L), this);

    time = seekOneNearest<Model*>("time");

    DistributedDelay::Parameters p;
    p.L = L;
    p.k = k;
    p.minIter = 1;
    delete dd;
    dd = new DistributedDelay(p);
}


void Stage::reset()
{
    dd->scale(0);
    sum = inflow = inflowPending = outflow = inflowTotal = outflowTotal = latestInflow = growth = 0;
    instantMortality = 0.;
    firstUpdate = true;
}

void Stage::update()
{
    applyInstantMortality();

    if (firstUpdate)
        inflow += initialInflow;

    inflowPending += inflow;
    inflowTotal += inflow;
    latestInflow = inflow;
    inflow = 0;

    dt = time->pullValue<double>("step");
    if (TestNum::eqZero(dt)) {
        sum = dd->state().content + inflowPending;
        outflow = growth = 0.;
        return;
    }

    if (fgr <= 0)
        throw Exception(QString("Growth rate (%1) must be > 0").arg(fgr), this);
    if (inflowPending < 0) {
        throw Exception(QString("Input (%1) must be >= 0").arg(inflowPending), this);
    }

    double realFgr = dd->findFgr(inflowPending, dt, fgr, sdRatio);
    dd->update(inflowPending, dt, realFgr);
    inflowPending = 0;
    firstUpdate = false;

    sum = dd->state().content;
    outflow = dd->state().outflowRate;
    growth = dd->state().growthRate;
}

void Stage::applyInstantMortality() {
    double survival = 1. - instantMortality/100.;
    TestNum::snapToZero(survival);
    TestNum::snapTo(survival, 1.);
    if (survival < 0 || survival > 1.)
       throw Exception(QString("Survival (%1) must be in the range [0;1]").arg(survival), this);
    dd->scale(survival);
    inflowPending *= survival;
    instantMortality = 0;
}

const double* Stage::data() {
    return dd->state().data;
}

double Stage::growthDemand() {
    DistributedDelay dd2(*dd);
    dd2.update(inflowPending, dt, fgr);
    return dd2.state().growthRate;
}

} // namespace
