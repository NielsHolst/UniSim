/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QString>
#include "simple_integrator.h"

namespace UniSim{
	
SimpleIntegrator::SimpleIntegrator(Identifier name, QObject *parent)
	: Integrator(name, parent), _runs(0), _steps(0)
{
}

void SimpleIntegrator::initialize()
{ 
    setParameter("numSteps", &_numSteps, 10);
}

void SimpleIntegrator::resetRuns()
{
    _runs = 0;
}

void SimpleIntegrator::resetSteps()
{
    _steps = 0;
}

bool SimpleIntegrator::nextRun()
{
	++_runs;
	return _runs == 1;
}

bool SimpleIntegrator::nextStep()
{
	++_steps;
	return _steps <= _numSteps;
}

} //namespace

