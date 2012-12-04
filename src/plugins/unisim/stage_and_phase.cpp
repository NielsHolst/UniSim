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
#include "distributed_delay_2d.h"
#include "stage_and_phase.h"
	
namespace UniSim {

StageAndPhase::StageAndPhase(UniSim::Identifier name, QObject *parent)
    : StageBase(name, parent), dd(0)
{
    new Parameter<int>("phaseK", &phaseK, 30, this,
        "Works like @F k but in the change phase process");
    new Parameter<double>("phaseDuration", &phaseL, 100., this,
        "Works like @F duration but in the change phase process");
    new ParameterVector<double>("inflow", &inflow, this,
        "Number of units to be put into the stage in the next time step");
    new Parameter<double>("timeStep", &timeStep, 1., this,
        "Time step for stage development, for example, in days or day-degrees");
    new Parameter<double>("phaseTimeStep", &phaseTimeStep, 1., this,
        "Time step for phase development, for example, in days or day-degrees");

    new VariableVector<double>("latestInflow", &latestInflow, this,
        "Inflow into the stage in latest time step");
    new VariableVector<double>("outflow", &outflow, this,
        "Outflow from the stage in latest time step");
}

DistributedDelayBase* StageAndPhase::createDistributedDelay() {
    DistributedDelay2D::FixedParameters p;
    p.L1 = L;
    p.L2 = phaseL;
    p.k1 = k;
    p.k2 = phaseK;
    p.minIter = 1;
    delete dd;
    return dd = new DistributedDelay2D(p, this);
}

void StageAndPhase::reset() {
    StageBase::reset();
    if (inflow.isEmpty()) {
        inflow.resize(phaseK);
        latestInflow.resize(phaseK);
    }
    else {
        inflow.fill(0.);
        latestInflow.fill(0.);
    }
    outflow.resize(phaseK);

    if (phaseInflow.isEmpty())
        phaseInflow.resize(k);
    phaseOutflow.resize(k);

    inflowPending.resize(phaseK);
    phaseInflowPending.resize(k);
}

void StageAndPhase::update() {
//    std::cout << qPrintable(id().label()) << " StageAndPhase::update() A\n";
    if (inflow.size() != phaseK) {
        QString msg("inflow's size (phaseK=%1) does not match the size of the recipient (phaseK=%2)");
        throw Exception(msg.arg(inflow.size()).arg(phaseK), this);
    }
    if (phaseInflow.size() != k) {
        QString msg("phaseInflow's size (k=%1) does not match the size of the recipient (k=%2)");
        throw Exception(msg.arg(phaseInflow.size()).arg(k), this);
    }

    applyInstantMortality();

    increment(inflowPending, inflow);
    latestInflow = inflow;
    inflowTotal += accum(inflow);

    increment(phaseInflowPending, phaseInflow);
    latestPhaseInflow = phaseInflow;
    phaseInflowTotal += accum(phaseInflow);

    // It remains to consider cases, when only one time step is zero
    if (TestNum::eqZero(timeStep) || TestNum::eqZero(phaseTimeStep)) {
        sum = dd->content() + accum(inflowPending) + accum(phaseInflowPending);
        outflow.fill(0.);
        phaseOutflow.fill(0.);
        growth = 0.;
        return;
    }

    DistributedDelay2D::UpdateParameters p;
    p.dt1 = timeStep;
    p.dt2 = phaseTimeStep;
    p.inflow1 = &inflowPending;
    p.inflow2 = &phaseInflowPending;
    dd->update(p);

    inflowPending.fill(0.);
    phaseInflowPending.fill(0.);

    sum = dd->content();
    outflow = dd->state().outflow1;
    phaseOutflow = dd->state().outflow2;

    outflowTotal += accum(outflow);
    phaseOutflowTotal += accum(phaseOutflow);
    growth = dd->state().growthRate;
//    std::cout << qPrintable(id().label()) << " StageAndPhase::update() Z\n";
}

double StageAndPhase::growthDemand() {
    return 0;
}

} // namespace
