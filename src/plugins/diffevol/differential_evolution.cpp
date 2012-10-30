/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
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
    new Parameter<double>("a", &a, 0., this, "description");
    new Parameter<double>("b", &b, 0., this, "description");

    new Variable<double>("deviation", &deviation, this, "description");
    new Variable<bool>("found", &found, this, "description");
    new Variable<bool>("passedMaxSteps", &passedMaxSteps, this, "description");
}

void DifferentialEvolution::initialize() {
    Integrator::initialize();
    distribution = new Distribution(-3,3);
    variate = new Variate(*randomGenerator(), *distribution);
    collectSamples();
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
    if (stopInNextStep)
        return false;
    else
        stopInNextStep = found || passedMaxSteps;
    return true;
}

void DifferentialEvolution::collectSamples() {
    // Find all plot-years
    QList<Year*> plotYears = seekMany<Year*>("*");

    // Pull pointers to parameters inside plot and plot-year
    samples.clear();
    for (int i = 0; i < plotYears.size(); ++i) {
        Year *plotYear = plotYears[i];
        Plot *plot = plotYear->seekParent<Plot*>("*");
        Sample s;
        s.area = plot->pullValuePtr<double>("Area");
        s.slope = plot->pullValuePtr<double>("Slope");
        s.fertiliser = plotYear->pullValuePtr<double>("Fertiliser");
        s.fungicide = plotYear->pullValuePtr<double>("Fungicide");
        s.yield = plotYear->pullValuePtr<double>("Yield");
        samples.append(s);
    }
}

void DifferentialEvolution::computeDeviation() {
    // sample some random regression coefficients
    a = (*variate)();
    b = (*variate)();

    // accumulate deviation from actual yield for all samples
    deviation = 0.;
    for (int i = 0; i < samples.size(); ++i) {
        Sample s = samples[i];
        double predictedYield = a*(*s.fertiliser) + b*(*s.fungicide);
        deviation += fabs(predictedYield - *s.yield);
    }
}

} //namespace

