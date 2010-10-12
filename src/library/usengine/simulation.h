/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_SIMULATION_H
#define UNISIM_SIMULATION_H

#include <QObject>
#include <QList>
#include <usbase/exception.h>
#include <usbase/identifier.h>

namespace UniSim{

class Integrator;
class Component;
class Model;
class Output;
class SimulationMaker;

class Simulation : public QObject
{
	Q_OBJECT
public:
	//! This enum describes the current state of the simulation
	enum State {
		Uninitialized,  //!< Child components have not been initialized
		Initialized, 	//!< Child components have been initialized
		Faulty			//!< Some error occured
	};
		
	Simulation(QString name, QString version="1.0", QObject *parent=0);
    void initialize(const Identifiers &sequence, SimulationMaker *simMaker = 0);
    virtual void execute();
	QString version() const;
	State state() const;
    const QList<Model*>& sequence() const;

    int runCount() const;
	int stepCount() const;
    void stopCurrentRun();
    void stopAllRuns();

protected:
    Integrator *_integrator;

private:
    QList<Model*> _models;  // models ordered in sequence of execution
    QList<Output*> _outputs;
    State _state;
	QString _version;
	bool _stopCurrentRun, _stopAllRuns;
	int _runCount, _stepCount;
};

} //namespace

#endif
