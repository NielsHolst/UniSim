/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QString>
#include "simple_controller.h"

namespace UniSim{
	
SimpleController::SimpleController(Identifier name, QObject *parent)
	: Controller(name, parent), _runs(0), _steps(0)
{
}

void SimpleController::initialize()
{ 
    setParameter("numSteps", &_numSteps, 10);
}

void SimpleController::resetRuns()
{
    _runs = 0;
}

void SimpleController::resetSteps()
{
    _steps = 0;
}

bool SimpleController::nextRun()
{
	++_runs;
	return _runs == 1;
}

bool SimpleController::nextStep()
{
	++_steps;
	return _steps <= _numSteps;
}

} //namespace

