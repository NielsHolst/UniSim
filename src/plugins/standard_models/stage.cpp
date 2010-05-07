/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <cmath>
#include <QString>
#include <QTextStream>
#include <usbase/exception.h>
#include <usbase/utilities.h>
#include "stage.h"
	
namespace UniSim {

Stage::Stage(UniSim::Identifier name, QObject *parent)
	: UniSim::Model(name, parent)
{
	_x = 0;
	_sum = _input = _output = _inputTotal = _outputTotal = 0;
	_firstUpdate = true;
	_dirtySum = true;

	setState("number", &_sum);
	setState("inflow", &_input);
	setState("outflow", &_output);
	setState("inflowTotal", &_inputTotal);
	setState("outflowTotal", &_outputTotal);
    setState("growthRate", &_fgr);
    setState("timeStep", &_dt);
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
	setParameter("k", &_k, 30);
	setParameter("duration", &_L, 100.);

    time = seekOneChild<Model*>("time");
}


void Stage::reset()
{
	if (_x!=0) delete[] _x;
	_x = new double[_k];
	fill(0);

	_sum =_input = _output = _inputTotal = _outputTotal = 0;
	_firstUpdate = true;
	_dirtySum = false;

	setInput("inflow", 0.);
	setInput("growthRate", 1.);
    setInput("instantMortality", 0.);
}

void Stage::update()
{
    applyInstantMortality();

    _input += input("inflow");
	_inputTotal += input("inflow");
	_fgr = input("growthRate");
	setInput("inflow", 0);

    double dt = time->state("step");
    _dt = dt;

	if (_k<=0 || _L<=0 || _fgr<=0 || _input< 0 || dt<0 || _x==0) {
		QString msg;
		QTextStream text(&msg);
        text << qPrintable(objectName()) << " in illegal state for update: \n"
			 << "k = "<< _k << "\n"
			 << "duration = "<< _L << "\n"
			 << "growth rate = " << _fgr << "\n"
			 << "_inflow = " <<_input << "\n"
			 << "time step = " << dt << "\n";
		if (!_x)
			text << "internal buffer is not ok\n";
		else
			text << "internal buffer is ok\n";
		throw UniSim::Exception(msg);
	}

	if (dt == 0) return;

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
    double survival = 1. - input("instantMortality")/100.;
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
    setInput("instantMortality", 0.);
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
