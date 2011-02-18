/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <cmath>
#include <QString>
#include <QTextStream>
#include <usbase/exception.h>
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include <usbase/push_variable.h>
#include <usbase/utilities.h>
#include "stage.h"
	
namespace UniSim {

Stage::Stage(UniSim::Identifier name, QObject *parent)
    : UniSim::Model(name, parent), _x(0), ageClassesPtr(0)
{
    new Parameter<int>("k", &_k, 30, this,
        "The number of age classes in the stage. The fewer age classes, the larger the variance on @F {duration}");
    new Parameter<double>("duration", &_L, 100., this,
        "The average duration of the stage: an inflow will emerge as an outflow dispersed "
        "over time, with a delay of @F duration on average and a variance of @Math {@F duration sup 2 slash @F k sup 2} "
        "@Cite{$label{(Manetsch 1976)}manetsch}.");
    new Parameter<double>("initialInflow", &initialInflow, 0., this,
        "The initial inflow is entered as inflow at time 0");

    // Next line can be used when pull variables have been templatized
    //new PullVariable<double>("ageClasses", &_x, this);
    new PullVariable<double>("number", &_sum, this, "Number of units (e.g. individuals) in stage");
    new PullVariable<double>("inflow", &_input, this, "Inflow into the stage in latest time step");
    new PullVariable<double>("outflow", &_output, this, "Outflow from the stage in latest time step");
    new PullVariable<double>("inflowTotal", &_inputTotal, this, "Total inflow into the stage since beginning of the simulation");
    new PullVariable<double>("outflowTotal", &_outputTotal, this, "Total outflow from the stage since beginning of the simulation");
    new PullVariable<double>("growthRate", &_fgr, this, "Same as the @F growthRate push variable");
    new PullVariable<double>("timeStep", &_dt, this, "The latest time step applied to the stage");

    new PushVariable<double>("growthRate", &_fgr, this, "Finite growth rate through stage. "
                     "For every quantity @I x that enters as inflow, @I growthRate*x will emerge as outflow. "
                     "@F growthRate can be changed during the simulation but must be larger than zero."
                     "Use small values (e.g., @Math {10 sup {-6}}) instead of zero");
    new PushVariable<double>("inflow", &_inflow, this, "Number of units to be put into the stage in the next time step");
    new PushVariable<double>("instantMortality", &_instantMortality, this,
                     "The mortality [0..1] will be applied in the next time step, before the @F inflow is added");
}

Stage::~Stage()
{
	if (_x!=0) {
		delete[] _x;
		_x = 0;
	}
}

void Stage::initialize()
{
    time = seekOneNearest<Model*>("time");
}


void Stage::reset()
{
	if (_x!=0) delete[] _x;
	_x = new double[_k];

    if (ageClassesPtr) delete ageClassesPtr;
    ageClassesPtr = new PullVariable<double>("ageClasses", _x, this, "description");

    fill(0);

    _sum =_input = _output = _inputTotal = _outputTotal = 0;
	_firstUpdate = true;
	_dirtySum = false;

    _inflow  = initialInflow;
    _fgr = 1.;
    _instantMortality = 0.;
}

void Stage::update()
{
    applyInstantMortality();

    _input += _inflow;
    _inputTotal += _inflow;
    _inflow = 0;

    double dt = time->pullVariable<double>("step");
    _dt = dt;

    if (_fgr <= 0)
        throw Exception("Growth rate (" + QString::number(_fgr)+ ") must be larger than zero", this);
	if (_k<=0 || _L<=0 || _fgr<=0 || _input< 0 || dt<0 || _x==0) {
		QString msg;
		QTextStream text(&msg);
        text << "Stage in illegal state for update: \n"
			 << "k = "<< _k << "\n"
			 << "duration = "<< _L << "\n"
			 << "growth rate = " << _fgr << "\n"
			 << "_inflow = " <<_input << "\n"
			 << "time step = " << dt << "\n";
		if (!_x)
			text << "internal buffer is not ok\n";
		else
			text << "internal buffer is ok\n";
        throw UniSim::Exception(msg, this);
	}

    if (dt == 0) {
        sum();
        _output = 0.;
        return;
    }

	// Set del and attrition according to Vansickle
	double 	del = _L*pow(_fgr, -1./_k),
			atr = _k*(1./_L - 1./del);		// atr <= 0 always

	// Initialize previous del, if it is not
	if (_firstUpdate) {
		_prevDel = del;
		_firstUpdate = false;
	}

	// Calculate attrition factor
	double b = 1. + (del - _prevDel)/dt/_k + atr*_prevDel/_k;

	// Divide time step to increase precision
    int idt = (int) floor(1.5 + 2.*b*dt*_k/_prevDel);
	Q_ASSERT(idt >= 1);

	//std::cout << "idt: " << idt << '\n';

	// Correct inflow for divided time step
	_input /= idt;

	// Calculate flow coefficient
	double a = _k/_prevDel*dt/idt;
	Q_ASSERT(a > 0. && a <= 1.);

	// Remember current del
	_prevDel = del;

	// Integrate
	_output = 0;
	for (int j = 0; j < idt; j++){
		// Collect outflow
		_output += a*_x[_k-1];
		// Step backwards through age classes
		for (int i = _k-1; i > 0; i--)
			_x[i] += a*(_x[i-1] - b*_x[i]);
		// Finish with first age class; enter inflow into that
		_x[0] += _input - a*b*_x[0];
	}

	_outputTotal += _output;
	_input = 0;
	_dirtySum = true;

    sum();
}

void Stage::applyInstantMortality() {
    double survival = 1. - _instantMortality/100.;
    if (survival == 1.) return;
    if (fabs(survival) < 1e-9) survival = 0.;

    if (survival == 0.) {
        fill(0.);
    }
    else {
        Q_ASSERT(survival > 0 && survival < 1.);
        for (int i = 0; i < _k; ++i) _x[i] *= survival;
    }
    _dirtySum = true;
    _instantMortality = 0;
}

void Stage::fill(double value)
{
	for (int i = 0; i < _k; ++i) {
		_x[i] = 0.;
	}
}

double Stage::sum() const
{
	if (_dirtySum) {
		_sum = 0;
		for (int i = 0; i < _k; ++i) {
			_sum +=_x[i];
		}
		_sum += _input;
		_dirtySum = false;
	}
	return _sum;
}

const double* Stage::ageClasses() const {
    return _x;
}

} // namespace
