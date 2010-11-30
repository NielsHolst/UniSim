/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QProgressDialog>
#include <QString>
#include "exception.h"
#include "integrator.h"
#include "model.h"
#include "pull_variable.h"

namespace UniSim{
	
Integrator::Integrator(Identifier name, QObject *parent)
    : Model(name, parent), report(0)
{
    new PullVariable<int>("stepNumber", &stepNumber, this, "Number of current time step in this iteration");
    new PullVariable<int>("runNumber", &runNumber, this, "Number of current iteration");
}

void Integrator::initialize() {
    Models models = seekChildren<Model*>("RunIterator");
    if (models.size() == 0)
        runIterator = 0;
    else if (models.size() == 1)
        runIterator = models[0];
    else
        throw Exception("Max. one child model named 'RunIterator' is allowed", this);

    runNumber = 0;
    reporting = cancelled = false;
}

void Integrator::reset() {
    stepNumber = 0;
}

bool Integrator::nextRun() {
    reportProgress();
    ++runNumber;
    bool nextOk;
    if (cancelled)
        nextOk = false;
    else if (runIterator)
        nextOk = runIterator->pullVariable<bool>("value");
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
        numRuns = runIterator->pullVariable<int>("numIterations");
    report = new QProgressDialog("Computing...", "Cancel simulation", 0, numRuns);
    report->setWindowModality(Qt::WindowModal);
    report->setMinimumDuration(1000);
    connect(report, SIGNAL(canceled()), this, SLOT(doCancel()));
}

void Integrator::closeReport() {
    report->close();
    reporting = false;
}

} //namespace

