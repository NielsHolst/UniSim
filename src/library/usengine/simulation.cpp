/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QMessageBox>
#include <QListIterator>
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

Simulation *Simulation::theSimulation = 0;

Simulation::Simulation(Identifier name)
    : NamedObject(name, 0)
{
    theSimulation = this;
    setProperty("classLabel", "Simulation");
}

Simulation::~Simulation() {
    theSimulation = 0;
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
    Integrator *integrator = seekOneChild<Integrator*>("*");
    QList<Model*> models = seekChildren<Model*>("*");       //integrator is also a model
    QList<Output*> outputs = seekChildren<Output*>("*");
    QListIterator<Model*> mo(models);
    QListIterator<Output*> ou(outputs);

    mo.toFront();
    while (mo.hasNext())
        mo.next()->deepInitialize();
    ou.toFront();
    while (ou.hasNext())
        ou.next()->deepInitialize();

    _state = Initialized;
    _runCount = _stepCount = 0;
	_stopCurrentRun = _stopAllRuns = false;
	
    while (integrator->nextRun() && !_stopAllRuns) {
        ++_runCount;
		_stepCount = 0;
		
        mo.toFront();
        while (mo.hasNext())
            mo.next()->deepReset();
        ou.toFront();
        while (ou.hasNext())
            ou.next()->deepReset();

        while (integrator->nextStep() && !_stopCurrentRun) {
			++_stepCount;
            mo.toFront();
            while (mo.hasNext())
                mo.next()->deepUpdate();
            ou.toFront();
            while (ou.hasNext())
                ou.next()->deepUpdate();
        }

            mo.toFront();
            while (mo.hasNext())
                mo.next()->deepCleanup();
            ou.toFront();
            while (ou.hasNext())
                ou.next()->deepCleanup();
    }

    mo.toFront();
    while (mo.hasNext())
        mo.next()->deepDebrief();
    ou.toFront();
    while (ou.hasNext())
        ou.next()->deepDebrief();
}

//! Gets version number
QString Simulation::version() const {
    return _version;
}

//! Get state of the simulation
Simulation::State Simulation::state() const {
    return _state;
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
    filePath = filePath;
    fileFolder = QFileInfo(filePath).absoluteDir();
}

//! Return the full path for the input file with given file name
/*! The seach order of findNearestFile is used
*/
QString Simulation::inputFilePath(QString fileName) {
    return findNearestFile(fileFolder, "input", fileName).absoluteFilePath();
}

//! Return the current simulation object
/*! An exception will be thrown if there is no current simulation object
*/
Simulation* simulation() {
    if (!Simulation::theSimulation)
        throw("No current simulation");
    return Simulation::theSimulation;
}

} //namespace
 

