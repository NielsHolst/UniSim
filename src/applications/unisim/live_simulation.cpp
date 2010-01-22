/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include <iostream>
#include <QDir>
#include <QTimer>
#include <usbase/file_locations.h>
#include <usengine/simulation.h>
#include <usengine/simulation_maker.h>
#include "graph_generator.h"
#include "live_simulation.h"
#include "log_base.h"

QMap<LiveSimulation::State, QString> LiveSimulation::stateTexts;

LiveSimulation::LiveSimulation(QObject *parent, LogBase *log_)
        : QObject(parent), log(log_),
        _state(Closed), _simulation(0), graphProcess(0)
{
    simulationMaker = new UniSim::SimulationMaker();

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
    delete graphProcess;
}

void LiveSimulation::open(QString filePath) {
    checkFilePath(filePath);
    if (_state != Ready) {
        parseFile();
	}
	else {
        LogBase::LogItem message =
            { "Warning", "Close current simulation before opening another" };
        log->tell(message);
    }
}

void LiveSimulation::checkFilePath(QString filePath_) {
    Q_ASSERT_X(QDir::isAbsolutePath(filePath_), "LiveSimulation::beginOpening",
               qPrintable("Must have absolute file path, got '" + filePath_ + "'"));

    filePath = filePath_;
    LogBase::LogItem message = {"Opening file", filePath};
    log->tell(message);
}

void LiveSimulation::parseFile() {
    Q_ASSERT(!_simulation);
    try {
        changeState(Opening);
        _simulation = simulationMaker->parse(filePath);
        changeState(EndOpening);
    }
    catch (UniSim::Exception &ex) {
        _simulation = 0;
        LogBase::LogItem message = { "Error", ex.message() };
        log->tell(message);
        changeState(Error);
    }
    catch(...) {
        _simulation = 0;
        LogBase::LogItem message =
            { "Error", "An unknown error occured when parsing " + filePath };
        log->tell(message);
        changeState(Error);
    }
}

void LiveSimulation::writeGraph() {
	State prevState = _state;
	changeState(ViewBuilding);

	if (prevState == Ready) {
        createGraphProcess();
	}
	else {
        LogBase::LogItem message =
            { "Warning", "Simulation not ready (maybe not opened or in error?)" };
        log->tell(message);
    	changeState(prevState);
    }
}

void LiveSimulation::createGraphProcess() {
    try {
        bool noProcess = establishNullProcess(graphProcess);
        if (noProcess) {
            GraphGenerator graphGenerator(_simulation, graphFilePath());
            graphProcess = graphGenerator.generate();
            connect(graphProcess, SIGNAL(stateChanged (QProcess::ProcessState)),
                    this, SLOT(graphProcessChanged(QProcess::ProcessState)));
        }
        else {
            changeState(Error);
        }
    }
    catch (UniSim::Exception &ex) {
        LogBase::LogItem message = { "Error", ex.message() };
        log->tell(message);
        changeState(Error);
    }
    catch(...) {
        LogBase::LogItem message =
            { "Unknown error", "Unknown error when building graph: " + graphFilePath() };
        log->tell(message);
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
            LogBase::LogItem message =
                { "Error", "Already generating a view of this simulation.\nPlease wait" };
            log->tell(message);
            return false;
        }
    }
    return true;
}

QString LiveSimulation::graphFilePath() const {
    QString path;
    QDir dir = UniSim::FileLocations::location(UniSim::FileLocations::Temporary);
    QString fileName = QFileInfo(filePath).fileName();
    path = dir.absolutePath() + "/" + fileName;
    path.replace(".xml", ".png");
    return path;
}

void LiveSimulation::graphProcessChanged(QProcess::ProcessState processState)
{
    if (processState == QProcess::NotRunning)
        changeState(EndViewBuilding);
}

void LiveSimulation::run() {
    if (_state != Ready) {
        LogBase::LogItem message =
            { "Warning", "Simulation not ready  (may  not opened or in error?)" };
        log->tell(message);
        return;
    }
    try {
        changeState(Running);
        _simulation->execute();
        changeState(EndRunning);
    }
    catch (UniSim::Exception &ex) {
        LogBase::LogItem message = { "Error", ex.message() };
        log->tell(message);
        changeState(Error);
    }
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
	
