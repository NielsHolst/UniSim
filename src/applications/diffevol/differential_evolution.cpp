/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include "differential_evolution.h"
#include "plot.h"
#include "year.h"

using namespace UniSim;

namespace diffevol {
	
DifferentialEvolution::DifferentialEvolution(Identifier name, QObject *parent)
    : Integrator(name, parent)
{
    new Parameter<int>("maxSteps", &maxSteps, 30, this, "description");
    new Parameter<double>("precision", &precision, 5., this, "description");
    new Parameter<QString>("outputFileName", &outputFileName, QString("diffevol_output.txt"), this, "description");
    new Parameter<double>("fert", &coeff.fert, 0.01, this, "description");
    new Parameter<double>("fung", &coeff.fung, -0.0031, this, "description");
    new Parameter<double>("intercept", &coeff.intercept, 0.2141+7.8704, this, "description");

    new Variable<double>("deviation", &deviation, this, "description");
    new Variable<bool>("found", &found, this, "description");
    new Variable<bool>("passedMaxSteps", &passedMaxSteps, this, "description");
}

void DifferentialEvolution::initialize() {
    Integrator::initialize();
    distribution = new Distribution(-0.5,0.5);
    variate = new Variate(*randomGenerator(), *distribution);
    collectPlots();
}

void DifferentialEvolution::collectPlots() {
    // Find all plot-years
    QList<Year*> plotYears = seekMany<Year*>("*");

    // Pull values of parameters inside plot and plot-year
    genome.clear();
    for (int i = 0; i < plotYears.size(); ++i) {
        Year *plotYear = plotYears[i];
        Plot *plot = plotYear->seekParent<Plot*>("*");
        Allele a;
        a.name = plot->id().label() + "_" + plotYear->id().label();
        a.area = plot->pullValue<double>("Area");
        a.slope = plot->pullValue<double>("Slope");
        a.fertiliser = plotYear->pullValue<double>("Fertiliser");
        a.fungicide = plotYear->pullValue<double>("Fungicide");
        a.yield = plotYear->pullValue<double>("Yield");
        genome.append(a);
    }
}

void DifferentialEvolution::reset() {
    Integrator::reset();
    found = passedMaxSteps = stopInNextStep = false;
}

bool DifferentialEvolution::nextStep() {
    ++stepNumber;
    progress = double(stepNumber)/maxSteps;

    computeDeviation();

    found = deviation <= precision;
    passedMaxSteps = stepNumber > maxSteps;
    if (stopInNextStep) {
        writeOutput();
        return false;
    }
    else
        stopInNextStep = found || passedMaxSteps;
    return true;
}

void DifferentialEvolution::computeDeviation() {
    // let genome drift randomly
    drift();

    // accumulate deviation from actual yield for all samples
    deviation = 0.;
    for (int i = 0; i < genome.size(); ++i) {
        Allele a = genome[i];
        double predictedYield = coeff.fert*a.fertiliser + coeff.fung*a.fungicide + coeff.intercept;
        deviation += predictedYield - a.yield;
    }
}

void DifferentialEvolution::drift() {
    for (int i=0; i < genome.size(); ++i) {
        drift(genome[i]);
    }
}

void DifferentialEvolution::drift(Allele &a) {
    a.fertiliser *= 1. + (*variate)();
    a.fungicide  *= 1. + (*variate)();
    a.fertiliser += (*variate)();
    a.fungicide  += (*variate)();
}

void DifferentialEvolution::writeOutput() {
    // Open file
    QString path = FileLocations::location(FileLocationInfo::Output).absolutePath();
    QString filePath = path + "/" + outputFileName;
    QFile file;
    file.setFileName(filePath);
    if (!file.open(QIODevice::Text | QIODevice::WriteOnly))
        throw Exception("Could not open output file: '" + filePath + "'");

    // Write genome
    file.write("Name\tFert\tFung\n");
    for (int i = 0; i < genome.size(); ++i) {
        Allele a = genome[i];
        QString s;
        QTextStream text(&s);
        text << a.name << "\t"
             << a.fertiliser << "\t"
             << a.fungicide << "\n";
        file.write(qPrintable(s));
    }
}

} //namespace

