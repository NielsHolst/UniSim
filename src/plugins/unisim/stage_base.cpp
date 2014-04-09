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
#include "stage_base.h"
	
namespace UniSim {

StageBase::StageBase(UniSim::Identifier name, QObject *parent)
    : Model(name, parent), ddBase(0)
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
    new Parameter<double>("sdRatio", &sdRatio, 1., this,
        "Supply/Demand ratio");
    new Parameter<double>("instantMortality", &instantMortality, 0., this,
        "Mortality [0..100] will be applied in the next time step, before @F inflow is added");
    new Parameter<double>("instantLossRate", &instantLossRate, 0., this,
        "Works just like @F mortality except the scale is a ratio [0..1]");
    new ParameterVector<double>("phaseInflow", &phaseInflow, this,
        "Inflow of dimension @F {k}");

    new Variable<double>("value", &sum, this,
        "Number of units (e.g. individuals) in stage");
    new Variable<double>("number", &sum, this,
        "Synonymous with @F {value}");
    new Variable<double>("inflowTotal", &inflowTotal, this,
        "Total inflow into the stage since beginning of the simulation");
    new Variable<double>("outflowTotal", &outflowTotal, this,
        "Total outflow from the stage since beginning of the simulation");
    new Variable<double>("phaseInflowTotal", &phaseInflowTotal, this,
        "Total phase change inflow into the stage since beginning of the simulation");
    new Variable<double>("phaseOutflowTotal", &phaseOutflowTotal, this,
        "Total phase change outflow from the stage since beginning of the simulation");
    new Variable<double>("growthIncrement", &growth, this,
        "Increment realised in this integration step");
    new VariableVector<double>("phaseOutflow", &phaseOutflow, this,
        "Outflow of dimension @F {k}");
}

StageBase::~StageBase() {
    delete ddBase;
}

void StageBase::initialize()
{
    if (k <= 0)
        throw Exception(QString("k (%1) must be > 0").arg(k), this);

    if (L <= 0)
        throw Exception(QString("Duration (%1) must be > 0").arg(L), this);

//    time = seekOneNearest<Model*>("time");
    ddBase = createDistributedDelay();
}


void StageBase::reset()
{
    Q_ASSERT(ddBase);
    ddBase->scale(0);
    sum = inflowTotal = outflowTotal = phaseInflowTotal = phaseOutflowTotal = growth = 0.;
    phaseOutflow.resize(k);
}

void StageBase::applyInstantMortality() {
    if (instantMortality > 0. && instantLossRate > 0.) {
        QString msg = "Parameters instantMortality and instantLossRate cannot both be > 0 (they are '%1' and '%2')";
        throw Exception(msg.arg(instantMortality).arg(instantLossRate));
    }
    double survival = 1. - instantMortality/100. - instantLossRate;
    TestNum::snapToZero(survival);
    TestNum::snapTo(survival, 1.);
    if (survival < 0 || survival > 1.)
       throw Exception(QString("Survival (%1) must be in the range [0;1]").arg(survival), this);
    ddBase->scale(survival);
    instantMortality = instantLossRate = 0;
}


const double* StageBase::data() {
    return ddBase->data();
}

} // namespace
