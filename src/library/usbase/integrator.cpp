/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QApplication>
#include <QMainWindow>
#include <QProgressDialog>
#include <QString>
#include "exception.h"
#include "integrator.h"
#include "main_window_interface.h"
#include "model.h"
#include "object_pool.h"
#include "variable.h"

namespace UniSim{
	
Integrator::Integrator(Identifier name, QObject *parent)
    : Model(name, parent), mainWindow(0), report(0)
{
    new Variable<int>("stepNumber", &stepNumber, this, "Number of current time step in this iteration");
    new Variable<double>("progress", &progress, this, "Progress of current iteration [0,1]");
    new Variable<int>("runNumber", &runNumber, this, "Number of current iteration");
    // This make the application task hang in memory
    //connect(QApplication::instance(), SIGNAL(lastWindowClosed()), this, SLOT(closeReport()));
}

void Integrator::amend() {
    try {
        mainWindow = objectPool()->find<QMainWindow*>("mainWindow");
    }
    catch (Exception &) {
        mainWindow = 0;
    }
}

void Integrator::initialize() {
    runIterator = peekOneChild<Model*>("RunIterator");
    runNumber = 0;
    reporting = cancelled = false;
}

void Integrator::reset() {
    stepNumber = 0;
	progress = 0.;
}

bool Integrator::nextRun() {
    if (mainWindow)
        reportProgress();
    ++runNumber;
    bool nextOk;
    if (cancelled) {
        nextOk = false;
    }
    else if (runIterator)
        nextOk = runIterator->pullValue<bool>("value");
    else
        nextOk = runNumber == 1;
    if (!nextOk)
        closeReport();
    return nextOk;
}

void Integrator::doCancel() {
    cancelled = true;
}

void Integrator::reportProgress() {
    if (!reporting) {
        openReport();
        reporting = true;
    }
    updateReport();
}

void Integrator::openReport() {
    if (!report)
        createReport();
}

void Integrator::updateReport() {
    report->setValue(runNumber);
}

void Integrator::createReport() {
    int numRuns = 1;
    if (runIterator)
        numRuns = runIterator->pullValue<int>("numIterations");
    report = new QProgressDialog("Computing...", "Cancel simulation", 0, numRuns, mainWindow);
    report->setWindowModality(Qt::WindowModal);
    report->setMinimumDuration(1000);
    connect(report, SIGNAL(canceled()), this, SLOT(doCancel()));
}

void Integrator::closeReport() {
    if (report) report->close();
    reporting = false;
}

void Integrator::acceptException(Exception *) {
    closeReport();
}

} //namespace

