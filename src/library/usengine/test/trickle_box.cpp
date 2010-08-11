#include <cmath>
#include <usbase/pull_variable.h>
#include <usbase/push_variable.h>
#include <iostream>
#include "trickle_box.h"

using namespace UniSim;

TrickleBox::TrickleBox(QString name, QObject *parent)
	: UniSim::Model(name,parent) 
{ 
    new PushVariable("inflow", &_inflow, this, "desc");
    new PullVariable<double>("contents", &_contents, this, "desc");
    new PullVariable<double>("outflow", &_outflow, this, "desc");
}

void TrickleBox::initialize()
{
     setParameter("capacity", &_capacity, 5, "desc");
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



