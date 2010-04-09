/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_INTEGRATOR_H
#define UNISIM_INTEGRATOR_H

#include <QObject>
#include "identifier.h"
#include "parameters.h"

namespace UniSim{
	
class Integrator : public QObject, public UniSim::Parameters
{
	Q_OBJECT
public:
    Integrator(Identifier name, QObject *parent=0);
	
	//! Will be called once, before the first execution of the simulation.
    virtual void initialize() { }

    //! Resets the state of the nextRun() guard function
    virtual void resetRuns() { }

    //! Resets the state of the nextStep() guard function
    virtual void resetSteps() { }
	
	//! Guard function that determines whether the whole Simulation should be run (again)
	virtual bool nextRun() = 0;
	
	//! Guard function that determines whether the Simulation should proceed one more time step
	virtual bool nextStep() = 0;
};

} //namespace

#endif
