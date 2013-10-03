/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include <usbase/exception.h>
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include "deposition_flush.h"
#include "pollen_onset_date.h"

using namespace UniSim;


namespace ecotox {

DepositionFlush::DepositionFlush(UniSim::Identifier name, QObject *parent)
    : Model(name, parent), pollenOnsetDate(0)
{
    new Parameter<double>("duration", &durationAsDouble, 14., this,
                          "Duration (days) of pollen release from crop");
    new Parameter<QString>("onsetFileName", &onsetFileName, QString(), this,
                           "Optional file with dates of pollen onset. "
                           "Date will be picked at random within calendar year");

    new Variable<double>("value", &value, this,
                           "Deposition rate (0..1)");
    new Variable<double>("total", &total, this,
                          "Accumulated deposition total (0..1)");
}

DepositionFlush::~DepositionFlush() {
    delete pollenOnsetDate;
}

void DepositionFlush::amend() {
    Model *depositionRate = peekOneSibling<Model*>("depositionRate");
    if (depositionRate) {
        if (!onsetFileName.isEmpty()) {
            QString msg = "If you have a 'depositionRate'' model in Pollen, you cannot also set the 'onsetFileName' parameter";
            throw Exception(msg);
        }
        pollenOnsetDate = new PollenOnsetDateFromCurve(depositionRate);
    }
    else {
        if (onsetFileName.isEmpty()) {
            QString msg = "You must either have a 'depositionRate'' model in Pollen or, set the 'onsetFileName' parameter";
            throw Exception(msg);
        }
        pollenOnsetDate = new PollenOnsetDateFromFile(onsetFileName);
    }

    loss = seekOneSibling<Model*>("lossRate");
    calendar = seekOne<Model*>("calendar");
}

void DepositionFlush::initialize() {
}

void DepositionFlush::reset() {
    duration = static_cast<unsigned int>(durationAsDouble + 0.5);
    value = total = 0.;
    calcScaling();
    onsetDate = pollenOnsetDate->calculate();
    phase = Before;
}

void DepositionFlush::calcScaling() {
    double peak = 0;
    for (int i = 1; i < duration; ++i) {
        if (f(i) > peak)
            peak = f(i);
    }
    scaling = 1./peak;

//    double sum = 0;
//    for (int i = 1; i < duration; ++i) {
//        sum += f(i);
//    }
//    double scaling1 = sum;

//    double eps = loss->pullValue<double>("rate");
//    double scaling2 = -50.0700*eps*eps*eps + 28.0483*eps*eps - 6.95117*eps + 1;

//    scaling = 1/scaling1/scaling2;
}

void DepositionFlush::update() {
    bool passedOnset = calendar->pullValue<QDate>("date") >= onsetDate;
    if (phase == Before && passedOnset) {
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

