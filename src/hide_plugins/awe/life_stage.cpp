/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <cmath>
#include <iostream>
#include "life_stage.h"

using namespace UniSim;
using namespace std;

LifeStage::LifeStage(QString name, QObject *parent)
	: Model(name,parent) 
{ 
	_x = 0;
	_fgr = 1.;
	_sum = _input = _output = _inputTotal = _outputTotal = 0;
	_firstUpdate = true;
	_dirtySum = true;
	
	setInput("input", 0.);
	
	setState("contents", &_sum);
	setState("fgr", &_fgr);
	setState("input", &_input);
	setState("output", &_output);
	setState("inputTotal", &_inputTotal);
	setState("outputTotal", &_outputTotal);
}

LifeStage::~LifeStage()
{
	if (_x!=0) {
		delete[] _x;
		_x = 0;
	}
}

void LifeStage::initialize()
{
	new Parameter<double>("k", &_k, 30);
	new Parameter<double>("duration", &_L, 100.);
    new Parameter<double>("initial_input", &_initialInput, 0.);
}

void LifeStage::reset()
{
	if (_x!=0) delete[] _x;
	_x = new double[_k];
	fill(0);

	_sum =_input = _output = _inputTotal = _outputTotal = 0;
	_firstUpdate = true;
	_dirtySum = false;

    setInput("input", _initialInput);
}

void LifeStage::update()
{
	_input += input("input");
	_inputTotal += input("input");
	setInput("input", 0);
	
	const double dt = 1;
		
	if (!(_k>0 && _L>0 && _fgr>0 && _input>= 0 && dt>=0 && _x)) {
		std::cout 
		<< qPrintable(objectName()) << "\n"
		<< _k << "\n"
		<< _L << "\n"
		<< _fgr << "\n"
		<< _input << "\n"
		<< dt << "\n";
		if (!_x) 
			std::cout << "_x not ok\n";
		else
			std::cout << "_x is ok\n";
	}
	
	Q_ASSERT(_k>0 && _L>0 && _fgr>0 && _input>= 0 && dt>=0 && _x);
	
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
	int idt = floor(1.5 + 2.*b*dt*_k/_prevDel);
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
	sum(); // update state
	//std::cout << "Update end: " << qPrintable(objectName()) << "\n";
}


void LifeStage::fill(double value)
{
	for (int i = 0; i < _k; ++i) {
		_x[i] = 0.;
	}
}

double LifeStage::sum() const
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


