#include <cmath>
#include <iostream>
#include "trickle_box.h"

TrickleBox::TrickleBox(QString name, QObject *parent)
	: UniSim::Model(name,parent) 
{ 
	setInput("inflow", 0.);
	setState("contents", &_contents);
	setState("outflow", &_outflow);
}

void TrickleBox::initialize()
{
	 setParameter("capacity", &_capacity, 5);
}

void TrickleBox::reset()
{
	_contents =_inflow = _outflow = 0;
}

void TrickleBox::update()
{
    _contents += input("inflow");
	if (_contents > _capacity)
		_outflow = _contents - _capacity;
}



