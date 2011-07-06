/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <cmath>
#include <QMessageBox>
#include <QString>
#include <QTextStream>
#include <usbase/test_num.h>
#include <usbase/exception.h>
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include <usbase/push_variable.h>
#include <usbase/utilities.h>
#include "stage.h"
	
namespace UniSim {

Stage::Stage(UniSim::Identifier name, QObject *parent)
    : UniSim::Model(name, parent), ageClassesPtr(0)
{
    new Parameter<int>("k", &k, 30, this,
        "The number of age classes in the stage. The fewer age classes, the larger the variance on @F {duration}");
    new Parameter<double>("duration", &_L, 100., this,
        "The average duration of the stage: an inflow will emerge as an outflow dispersed "
        "over time, with a delay of @F duration on average and a variance of @Math {@F duration sup 2 slash @F k sup 2} "
        "@Cite{$label{(Manetsch 1976)}manetsch}. @F duration is also a pull/push variable");
    new Parameter<double>("growthRate", &_fgr, 1., this,
        "For every quantity @I x that enters as inflow, @I growthRate*x will emerge as outflow. "
        "@F growthRate can be changed during the simulation but must be larger than zero."
        "Use small values (e.g., @Math {10 sup {-6}}) instead of zero. "
        "@F growthRate is also a pull/push variable");
    new Parameter<double>("initialInflow", &initialInflow, 0., this,
        "The initial inflow is entered as inflow at time 0");


    new PullVariable<double>("value", &_sum, this, "Number of units (e.g. individuals) in stage");
    new PullVariable<double>("number", &_sum, this, "Synonymous with @F {value}");
    new PullVariable<double>("inflow", &_prevInflow, this, "Inflow into the stage in latest time step");
    new PullVariable<double>("outflow", &_output, this, "Outflow from the stage in latest time step");
    new PullVariable<double>("inflowTotal", &_inputTotal, this, "Total inflow into the stage since beginning of the simulation");
    new PullVariable<double>("outflowTotal", &_outputTotal, this, "Total outflow from the stage since beginning of the simulation");
    new PullVariable<double>("timeStep", &_dt, this, "The latest time step applied to the stage");
    new PullVariable<double>("duration", &_L, this, "Same as @F duration parameter.");
    new PullVariable<double>("growthRate", &_fgr, this, "Same as the @F growthRate parameter.");

    new PushVariable<double>("duration", &_L, this, "Same as @F duration parameter. ");
    new PushVariable<double>("growthRate", &_fgr, this, "Same as the @F growthRate parameter.");
    new PushVariable<double>("inflow", &_inflow, this, "Number of units to be put into the stage in the next time step");
    new PushVariable<double>("instantMortality", &_instantMortality, this,
                     "The mortality [0..100] will be applied in the next time step, before the @F inflow is added");
}

void Stage::initialize()
{
    if (k <= 0)
        throw Exception(QString("k (%1) must be > 0").arg(k), this);
    if (_L <= 0)
        throw Exception(QString("Duration (%1) must be > 0").arg(_L), this);
    time = seekOneNearest<Model*>("time");
    buffer.fill(0., k);
    x = buffer.data();
    if (ageClassesPtr)
        delete ageClassesPtr;
    ageClassesPtr = new PullVariable<double>("ageClasses", x, this, "Pointer to buffer with age classes of length @F {k}");
}


void Stage::reset()
{
    buffer.fill(0);
    _sum = _inflow =_input = _output = _inputTotal = _outputTotal = _prevInflow = 0;
	_dirtySum = false;
    _instantMortality = 0.;
    firstUpdate = true;
}

void Stage::update()
{
    _dirtySum = true;
    applyInstantMortality();

    if (firstUpdate) {
//        Model *test = peekOneAscendant<Model*>("hibernating");
//        if (test)
//            QMessageBox::information(0, "test", "Update hibernating");
        _inflow += initialInflow;
        firstUpdate = false;
    }
    _input += _inflow;
    _inputTotal += _inflow;
    _prevInflow = _inflow;
    _inflow = 0;

    double dt = time->pullVariable<double>("step");
    _dt = dt;

    if (_fgr <= 0)
        throw Exception(QString("Growth rate (%1) must be > 0").arg(_fgr), this);
    if (_input < 0) {
        throw Exception(QString("Input (%1) must be >= 0").arg(_input), this);
    }

	// Set del and attrition according to Vansickle
    double 	del = _L*pow(_fgr, -1./k),
            atr = k*(1./_L - 1./del);		// atr <= 0 always

    // Time stands still
    if (dt == 0) {
        sum();
        _output = 0.;
        return;
    }

    // Calculate attrition factor
    double b = 1. + atr*del/k;

	// Divide time step to increase precision
    int idt = (int) floor(1.5 + 2.*b*dt*k/del);
	Q_ASSERT(idt >= 1);

	//std::cout << "idt: " << idt << '\n';

	// Correct inflow for divided time step
	_input /= idt;

	// Calculate flow coefficient
    double a = k/del*dt/idt;
	Q_ASSERT(a > 0. && a <= 1.);

	// Integrate
	_output = 0;
	for (int j = 0; j < idt; j++){
		// Collect outflow
        _output += a*x[k-1];
		// Step backwards through age classes
        for (int i = k-1; i > 0; i--)
            x[i] += a*(x[i-1] - b*x[i]);
		// Finish with first age class; enter inflow into that
        x[0] += _input - a*b*x[0];
	}

	_outputTotal += _output;
	_input = 0;
	_dirtySum = true;
    sum();
}

void Stage::applyInstantMortality() {
    double survival = 1. - _instantMortality/100.;
    TestNum::snapToZero(survival);
    TestNum::snapTo(survival, 1.);
    if (survival < 0 || survival > 1.)
       throw Exception(QString("Survival (%1) must be in the range [0;1]").arg(survival), this);

    if (survival == 0.)
           buffer.fill(0.);
    else if (survival == 1.)
        return;
    else
        for (int i = 0; i < k; ++i) x[i] *= survival;

    _dirtySum = true;
    _instantMortality = 0;
}

double Stage::sum() const
{
	if (_dirtySum) {
		_sum = 0;
        for (int i = 0; i < k; ++i) {
            _sum += x[i];
		}
		_sum += _input;
		_dirtySum = false;
	}
	return _sum;
}

QVector<double>* Stage::getBuffer() {
    return &buffer;
}


} // namespace
