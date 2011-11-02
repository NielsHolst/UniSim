/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <usbase/exception.h>
#include <usbase/model.h>
#include <usbase/integrator.h>
#include <usbase/output.h>
#include <usbase/utilities.h>
#include "simulation.h"
#include "simulation_maker.h"

/*! \class UniSim::Simulation
    \brief The %Simulation class holds and executes models

    To set up a %Simulation object you first create the components of the simulation as children of the
    simulation. You achieve this most easily bu defining the structure of the model and its components in
    an XML file from which SimulationMaker will create a %Simulation object and its components.

    You can also create all components in C++ via the constructors and put them all together in a simulation
    object. This is most useful for testing.

    The children of a Simulation object must comprise one Integrator, at least one Model and at least one Output,
*/

namespace UniSim{

Simulation::Simulation(Identifier name, QString version, QObject *parent)
    : NamedObject(name, parent), _state(Uninitialized), _version(version)
{
}

//! Sorts children and sets the sequence of model updating
/*! The children are seggregated into three types: Integrator, Model and Output. There must be one child of
    the first type and one or more of the others.

    If sequence is empty then only one top-level Model child is allowed. Otherwise sequence tells in
    which order the top-level models will be managed.

    \sa Component management functions

    After seggregation and sequencing all children are initialized through Component::initialize().

    The initialize() function can be effectuated only once for each %Simulation object. Subsequent calls
    will be ignored.

    \param sequence list of top-level models in the order they will be managed
*/
void Simulation::initialize(const Identifiers &sequence, SimulationMaker *simMaker)
{
	if (_state == Initialized)  return;
	
	_state = Faulty;
	_models.clear();
    _outputs.clear();

	// Segregate my children
	QList<Integrator*> integrators;
	QList<Model*> models;
	for (QList<QObject*>::const_iterator ch = children().begin(); ch != children().end(); ++ch) {
		Integrator *integrator = dynamic_cast<Integrator*>(*ch);
		Model *model = dynamic_cast<Model*>(*ch);
		Output *output = dynamic_cast<Output*>(*ch);
		if (integrator)
			integrators.append(integrator);
		else if (model)
			models.append(model);
		else if (output)
			_outputs.append(output);
	}
	Q_ASSERT(integrators.size() == 1);
	Q_ASSERT(models.size() > 0);
    Q_ASSERT(children().size() == integrators.size() + models.size() + _outputs.size());
	
	_integrator = integrators[0];

	// Put models in sequence		
    if (sequence.isEmpty()) {
		if (models.size() == 1) 
            _models.append(models[0]);
		else 
			throw Exception("Sequence of models must be specified in 'integrator' element");
	} 
	else {
        for (Identifiers::const_iterator se = sequence.begin(); se != sequence.end(); ++se) {
            _models.append(modelInstances(se->key()));
		}
	}

	// Initialize integrator, models and outputs
    for (Models::iterator mo = _models.begin(); mo != _models.end(); ++mo)  (*mo)->deepInitialize();
    if (simMaker) {
        simMaker->setupOutputVariableElements();
        simMaker->setupOutputParameterElements();
    }
    for (Outputs::iterator ou = _outputs.begin(); ou != _outputs.end(); ++ou) (*ou)->deepInitialize();

	_state = Initialized;
	_runCount = _stepCount = 0;
}

Models Simulation::modelInstances(QString modelName) {
    Models instances;
    Model *oneModel = peekOneChild<Model*>(modelName);
    if (oneModel) {
        instances << oneModel;
    }
    else {
        int i = 0;
        Model *instance;
        do {
            QString instanceName = modelName + "(" + QString::number(++i) + ")";
            instance = peekOneChild<Model*>(instanceName);
            if (instance)
                instances << instance;
        } while (instance);
    }
    return instances;
}

//! Executes one or more runs of the simulation as determined by the integrator
/*!	The integrator determines through nextRun() and nextStep() the two controlling loops of the simulation:
    - Before every run: first the integrator, then the top-level models and then the outputs are reset().
        - For each time step: first the top-level models then the outputs are update()'ed.
    - After every run: first the top-level models then the outputs are cleanup()'ed.
    - After the last run: first the top-level models then the outputs are debrief()'ed.
*/
void Simulation::execute()
{
    initialize(Identifiers()); // Try initializing with empty sequence at the least
    if (_state != Initialized)
		throw Exception("Simulation could not be inialized. Correct the model specification file(s).");

	_runCount = 0;
	_stopCurrentRun = _stopAllRuns = false;
	
    _integrator->deepInitialize();
    while (_integrator->nextRun() && !_stopAllRuns) {
        ++_runCount;
		_stepCount = 0;
		
        _integrator->deepReset();
        for (Models::iterator mo = _models.begin(); mo != _models.end(); ++mo)  (*mo)->deepReset();
        for (Outputs::iterator ou = _outputs.begin(); ou != _outputs.end(); ++ou)  (*ou)->deepReset();
		
        _integrator->deepReset();
        while (_integrator->nextStep() && !_stopCurrentRun) {
			++_stepCount;
            _integrator->deepUpdate();
            for (Models::iterator mo = _models.begin(); mo != _models.end(); ++mo)  (*mo)->deepUpdate();
            for (Outputs::iterator ou = _outputs.begin(); ou != _outputs.end(); ++ou)  (*ou)->deepUpdate();
		}

        _integrator->deepCleanup();
        for (Models::iterator mo = _models.begin(); mo != _models.end(); ++mo)  (*mo)->deepCleanup();
        for (Outputs::iterator ou = _outputs.begin(); ou != _outputs.end(); ++ou)  (*ou)->deepCleanup();
    }

    _integrator->deepDebrief();
    for (Models::iterator mo = _models.begin(); mo != _models.end(); ++mo)  (*mo)->deepDebrief();
    for (Outputs::iterator ou = _outputs.begin(); ou != _outputs.end(); ++ou)  (*ou)->deepDebrief();
}

//! Gets version number
QString Simulation::version() const {
	return _version;
}

//! Get state of the simulation
Simulation::State Simulation::state() const {
	return _state;
}

const QList<Model*>& Simulation::sequence() const {
    return _models;
}

//! Get number of runs simulated
int Simulation::runCount() const {
	return _runCount;
}
	
//! Get number of steps simulated in current run
int Simulation::stepCount() const {
	return _stepCount;
}

void Simulation::stopCurrentRun() {
    _stopCurrentRun = true;
}

void Simulation::stopAllRuns() {
    _stopAllRuns = _stopCurrentRun = true;
}

//! Set the file path of the simulation XML file
/*! Do not use this to open an XML parser. Use SimulationMaker::parse.
*/
void Simulation::setFilePath(QString filePath) {
    _filePath = filePath;
    _fileFolder = QFileInfo(filePath).absoluteDir();
}

//! Return the full path for the input file with given file name
/*! The file is searched in the order:
    1. Folder of the simulation XML file
    2. "input" sub-folder of that
    3. "input" sub-folder of the parent
    4. "input" sub-folder of the grandparent, and so forth
    If the file is not found, a file path in location 1 above is returned,
    in which case the returned path refers to a non-existing file
*/
QString Simulation::inputFilePath(QString fileName) {
    return findNearestFile(_fileFolder, "input", fileName).absoluteFilePath();
}

//! Return the current simulation object or null
Simulation* simulation() {
    Simulation *sim = dynamic_cast<Simulation*>(simulationObject());
    Q_ASSERT(sim);
    return sim;
}

} //namespace
 

