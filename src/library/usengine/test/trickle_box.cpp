#include <cmath>
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include <iostream>
#include "trickle_box.h"

using namespace UniSim;

TrickleBox::TrickleBox(QString name, QObject *parent)
	: UniSim::Model(name,parent) 
{ 
    new Parameter<int>("capacity", &_capacity, 5, this, "desc");
    new Parameter<double>("inflow", &_inflow, 0., this, "desc");
    new Variable<double>("contents", &_contents, this, "desc");
    new Variable<double>("outflow", &_outflow, this, "desc");
}

void TrickleBox::initialize()
{
}

void TrickleBox::reset()
{
	_contents =_inflow = _outflow = 0;
}

void TrickleBox::update()
{
    _contents += _inflow;
	if (_contents > _capacity)
		_outflow = _contents - _capacity;
}



