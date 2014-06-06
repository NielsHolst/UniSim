/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMainWindow>
#include <Qt>
#include "integrator.h"
#include "model.h"
#include "object_pool.h"

namespace UniSim{
	
Integrator::Integrator(Identifier name, QObject *parent)
    : Model(name, parent), progressDialog("Running simulation...", "Stop", 0, 100)
{
    Output(int, stepNumber); // Number of current time step in this iteration");
    Output(double, progress); // Progress of current iteration [0,1]");
    Output(int, runNumber); //Number of current iteration");
    setupProgressDialog();
}

void Integrator::setupProgressDialog() {
//    QMainWindow *mainWindow;
//    try {
//        mainWindow = objectPool()->find<QMainWindow*>("mainWindow");
//    }
//    catch (Exception &) {
//        mainWindow = 0;
//    }
//    progressDialog.setParent(mainWindow->centralWidget());
//    progressDialog.setAttribute(Qt::Window);
    progressDialog.setWindowModality(Qt::WindowModal);
}

void Integrator::initialize() {
    runIterator = peekOneChild<Model*>("runIterator");
    if (!runIterator)
        runIterator = peekOneChild<Model*>("iterator");
    runNumber = 0;
}

void Integrator::reset() {
    stepNumber = 0;
	progress = 0.;
}

bool Integrator::nextStep() {
    progressDialog.setValue((int) 100*progress);
    bool carryOn = doNextStep() && !progressDialog.wasCanceled();
    if (!carryOn) {
        progressDialog.setValue(100);
        progressDialog.reset();
    }
    return carryOn;
}

bool Integrator::nextRun() {
    if (progressDialog.wasCanceled())
        return false;
    ++runNumber;
    return runIterator ?
           runIterator->pullValue<bool>("value")
           : runNumber == 1;
}


} //namespace

