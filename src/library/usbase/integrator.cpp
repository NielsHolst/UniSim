/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <Qt>
#include "exception"
#include "integrator.h"

using namespace std;

namespace UniSim{
	
Integrator::Integrator(Identifier name, QObject *parent)
    : Model(name, parent)
{
    Input2(int, indicatorInt, indicator, 2);
    Output(int, stepNumber); // Number of current time step in this iteration");
    Output(double, progress); // Progress of current iteration [0,1]");
    Output(int, runNumber); //Number of current iteration");
}

void Integrator::initialize() {
    runIterator = peekOneChild<Model*>("runIterator");
    if (!runIterator)
        runIterator = peekOneChild<Model*>("iterator");
    runNumber = 0;
    if (0 <= indicatorInt && indicatorInt < 3)
        indicator = (Indicator) indicatorInt;
    else {
        QString msg{"Invalid indicator '%1'.Must be 0,1 or 2"};
        throw Exception(msg.arg(indicatorInt), this);
    }
    beginProgress();
}

void Integrator::reset() {
    stepNumber = 0;
	progress = 0.;
}

void Integrator::debrief() {
    endProgress();
}

bool Integrator::nextStep() {
    showProgress();
    bool carryOn = doNextStep() && !wasCanceled();
    if (!carryOn && indicator==Bar) {
        progressDialog->setValue(100);
        progressDialog->reset();
    }
    return carryOn;
}

bool Integrator::wasCanceled() {
    return indicator==Bar && progressDialog->wasCanceled();
}

bool Integrator::nextRun() {
    if (wasCanceled())
        return false;
    ++runNumber;
    return runIterator ?
           runIterator->pullValue<bool>("value")
           : runNumber == 1;
}

void Integrator::beginProgress() {
    switch (indicator) {
    case Bar:
        progressDialog = new QProgressDialog("Running simulation...", "Stop", 0, 100);
        progressDialog->setWindowModality(Qt::WindowModal);
        break;
    case Console:
        progressDialog = 0;
        cout << "Running...\n";
    case None:
        ;
    }
    prevProgress = 0.;
}

void Integrator::showProgress() {
    if (progress >= prevProgress+0.01) {
        prevProgress = progress;
        int percent = (int) 100*progress;
        switch (indicator) {
        case Bar:
            progressDialog->setValue(percent);
            break;
        case Console:
            cout << '\r' << percent ;
        case None:
            ;
        }
    }
}

void Integrator::endProgress() {
    switch (indicator) {
    case Bar:
        delete progressDialog;
        break;
    case Console:
        cout << "\rDone!\n";
    case None:
        ;
    }
}

} //namespace

