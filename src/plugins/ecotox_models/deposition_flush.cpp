/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "deposition_flush.h"

using namespace UniSim;


namespace ecotox {

DepositionFlush::DepositionFlush(UniSim::Identifier name, QObject *parent)
    : Model(name, parent), distribution(0), variate(0)
{
    new Parameter<int>("duration", &duration, 14, this,
                          "Duration (days) of pollen release from crop");

    new PullVariable<double>("value", &value, this,
                           "Deposition rate (0..1)");
    new PullVariable<double>("total", &total, this,
                          "Accumulated deposition total (0..1)");
}

DepositionFlush::~DepositionFlush() {
    delete distribution;
    delete variate;
}

void DepositionFlush::initialize() {
    Model *depositionRate = seekOneSibling<Model*>("depositionRate");
    depositionTotal = depositionRate->pullVariablePtr<double>("total");
    distribution = new Distribution;
    variate = new Variate(*randomGenerator(), *distribution);
}

void DepositionFlush::reset() {
    value = total = 0.;
    calcScaling();
    percentile = (*variate)();
    Q_ASSERT(percentile >= 0. && percentile < 1.);
    phase = Before;
}

void DepositionFlush::calcScaling() {
    double sum = 0;
    for (int i = 1; i < duration; ++i) {
        sum += f(i);
    }
    scaling = 1./sum;
}

void DepositionFlush::update() {
    bool passedPercentile = *depositionTotal >= percentile;
    if (phase == Before && passedPercentile) {
        phase = Inside;
        phaseTime = 0;
    }
    if (phase == Inside) {
        if (phaseTime == duration) {
            value = 0.;
            phase = After;
        }
        else {
            value = f(phaseTime++)*scaling;
            total += value;
        }
    }
}

} //namespace

