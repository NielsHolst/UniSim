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
#include <usbase/parameter_vector.h>
#include <usbase/variable.h>
#include <usbase/variable_vector.h>
#include <usbase/test_num.h>
#include <usbase/utilities.h>
#include "distributed_delay.h"
#include "publish.h"
#include "stage.h"
	
namespace UniSim {

PUBLISH(Stage)

Stage::Stage(UniSim::Identifier name, QObject *parent)
    : StageBase(name, parent), dd(0)
{
    addParameterRef<double>("timeStep", &dt, ".../time[step]");
    new Parameter<double>("inflow", &inflow, 0., this,
        "Number of units to be put into the stage in the next time step");
    new Parameter<double>("initialInflow", &initialInflow, 0., this,
        "The @F initialInflow is entered as inflow at time 0");
    new Parameter<double>("phaseOutflowProportion", &phaseOutflowProportion, 0., this,
        "Proportion that will change phase in next time step");

    new Variable<double>("latestInflow", &latestInflow, this,
        "Inflow into the stage in latest time step");
    new Variable<double>("outflow", &outflow, this,
        "Outflow from the stage in latest time step");
//    new Variable<double>("timeStep", &dt, this,
//        "The latest time step applied to the stage");
}

DistributedDelayBase* Stage::createDistributedDelay() {
    DistributedDelay::Parameters p;
    p.L = L;
    p.k = k;
    p.minIter = 1;
    delete dd;
    return dd = new DistributedDelay(p, this);
}

void Stage::reset() {
    StageBase::reset();
    inflowPending = dt = latestInflow = outflow = 0.;
    firstUpdate = true;
}

void Stage::update() {
    applyInstantMortality();

    latestInflow = 0.;
    if (firstUpdate) {
        inflowPending += initialInflow;
        inflowTotal += initialInflow;
        latestInflow += initialInflow;
        firstUpdate = false;
    }

    inflowPending += inflow;
    inflowTotal += inflow;
    latestInflow += inflow;

    if (!phaseInflow.isEmpty()) {
        if (phaseInflow.size() != k) {
            QString msg("phaseInflow's size (k=%1) does not match the size of the recipient (k=%2)");
            throw Exception(msg.arg(phaseInflow.size()).arg(k), this);
        }
        double *contents = const_cast<double*>(data());
        increment(contents, phaseInflow.data(), k);
        phaseInflowTotal += accum(phaseInflow);
    }

//    dt = time->pullValue<double>("step");
    if (TestNum::eqZero(dt)) {
        sum = dd->content() + inflowPending;
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

    if (phaseOutflowProportion == 0.) {
        phaseOutflow.fill(0.);
    }
    else{
        TestNum::snapTo(phaseOutflowProportion, 1.);
        if (phaseOutflowProportion > 1.) {
            QString msg = "phaseOutflowProportion = %1 must not be > 1";
            throw Exception(msg.arg(phaseOutflowProportion), this);
        }
        phaseOutflow = dd->take(phaseOutflowProportion);
        phaseOutflowTotal += accum(phaseOutflow);
    }

    sum = dd->content();
    outflowTotal += outflow = dd->state().outflowRate;
    growth = dd->state().growthRate;
}

double Stage::growthDemand() {
    if (TestNum::eqZero(dt))
        return 0.;
    DistributedDelay dd2(*dd);
    dd2.update(inflowPending, dt, fgr);
    return dd2.state().growthRate;
}

} // namespace
