/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include <iostream>
#include <QDir>
#include <QTimer>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usengine/simulation.h>
#include <usengine/simulation_maker.h>
#include "graph_generator.h"
#include "live_simulation.h"

using namespace UniSim;

QMap<LiveSimulation::State, QString> LiveSimulation::stateTexts;

LiveSimulation::LiveSimulation(QObject *parent)
        : QObject(parent), _state(Closed), _simulation(0)
{
    simulationMaker = new UniSim::SimulationMaker();

    graphProcesses.append(0);
    graphProcesses.append(0);

    connect(simulationMaker, SIGNAL(beginExpansion()), this, SLOT(acceptBeginExpansion()));
    connect(simulationMaker, SIGNAL(endExpansion()), this, SLOT(acceptEndExpansion()));

    connect(simulationMaker, SIGNAL(beginInitialization()), this, SLOT(acceptBeginInitialization()));
    connect(simulationMaker, SIGNAL(endInitialization()), this, SLOT(acceptEndInitialization()));

	setupStateTexts();   
}

void LiveSimulation::acceptBeginExpansion() {
    changeState(Expanding);
}

void LiveSimulation::acceptEndExpansion()  {
    changeState(EndExpanding);
}
void LiveSimulation::acceptBeginInitialization() {
    changeState(Initializing);
}

void LiveSimulation::acceptEndInitialization() {
    changeState(EndInitializing);
}

void LiveSimulation::setupStateTexts() {
    stateTexts[Closed] = "Closed";
    stateTexts[Ready] = "Ready";
    stateTexts[Error] = "Error";
    stateTexts[Opening] = "Opening...";
    stateTexts[EndOpening] = "Opening finished";
    stateTexts[Expanding] = "XML-expanding...";
    stateTexts[EndExpanding] = "XML-expanding finished";
    stateTexts[Initializing] = "Initializing...";
    stateTexts[EndInitializing] = "Initializing finished";
    stateTexts[ViewBuilding] = "View-building...";
    stateTexts[EndViewBuilding] = "View-building finished";
    stateTexts[Running] = "Running...";
    stateTexts[EndRunning] = "Running finished";
    stateTexts[Closing] = "Closing";
    Q_ASSERT(stateTexts.size() == int(Closing) + 1);
}

LiveSimulation::~LiveSimulation() {
    delete simulationMaker;
    delete graphProcesses[0];
    delete graphProcesses[1];
}

void LiveSimulation::open(QString filePath) {
    checkFilePath(filePath);
    if (_state == Ready)
        throw Exception("Close current simulation before opening another");
    parseFile();
}

void LiveSimulation::checkFilePath(QString filePath_) {
    Q_ASSERT_X(QDir::isAbsolutePath(filePath_), "LiveSimulation::beginOpening",
               qPrintable("Must have absolute file path, got '" + filePath_ + "'"));

    filePath = filePath_;
}

void LiveSimulation::parseFile() {
    Q_ASSERT(!_simulation);
    changeState(Opening);
    _simulation = simulationMaker->parse(filePath);
    changeState(EndOpening);
}

void LiveSimulation::writeGraph() {
    if (_state != Ready)
        throw Exception("Simulation not ready (maybe not opened or in error?)");
	changeState(ViewBuilding);
    createGraphProcess();
}

void LiveSimulation::createGraphProcess() {
    Q_ASSERT(graphProcesses.size() == 2);
    for (int i = 0; i < 2; ++i) {
        if (graphProcesses[i]) {
            delete graphProcesses[i];
            graphProcesses[i] = 0;
        }
    }
    _graphFilePath.clear();

    bool noProcess = establishNullProcess(graphProcesses[0]);
    if (noProcess) {
        GraphGenerator generator(_simulation);
        graphProcesses[0] = generator.generate(GraphGenerator::PNG);
        graphProcesses[1] = generator.generate(GraphGenerator::Postscript);

        _graphFilePath = generator.outputFilePath(GraphGenerator::PNG);
        connect(graphProcesses[0], SIGNAL(stateChanged (QProcess::ProcessState)),
                this, SLOT(graphProcessChanged(QProcess::ProcessState)));
    }
    else {
        changeState(Error);
    }
}

bool LiveSimulation::establishNullProcess(QProcess *process) {
    if (process) {
        if (process->state() == QProcess::NotRunning) {
            delete process;
            process = 0;
            return true;
        }
        else {
            throw Exception("Already generating a view of this simulation.\nPlease wait");
        }
    }
    return true;
}

QString LiveSimulation::graphFilePath() const {
    return _graphFilePath;
}

void LiveSimulation::graphProcessChanged(QProcess::ProcessState processState)
{
    if (processState == QProcess::NotRunning)
        changeState(EndViewBuilding);
}

void LiveSimulation::run() {
    if (_state != Ready)
        throw Exception("Simulation not ready (maybe not opened or in error?)");
    changeState(Running);
    _simulation->execute();
    changeState(EndRunning);
}

void LiveSimulation::close() {
	changeState(Closing);
    delete _simulation;
    _simulation = 0;
    changeState(Closed);
}

void LiveSimulation::changeState(State newState) {
    State previous = _state;
    _state = newState;
    emit stateChanged((int) previous, (int) _state);
    if (_state == EndOpening || _state == EndViewBuilding || _state == EndRunning)
		changeState(Ready);
}

LiveSimulation::State LiveSimulation::state() const {
	return _state;
}

QString LiveSimulation::stateText(State state) {
    return stateTexts[state];
}

UniSim::Simulation* LiveSimulation::simulation() {
	return _simulation;
}
	
