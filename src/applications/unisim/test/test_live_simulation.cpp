#include <iostream>
#include <QSignalSpy>
#include <usbase/integrator.h>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/utilities.h>
#include <usengine/simulation.h>
#include "../live_simulation.h"
#include "../log_base.h"
#include "test_live_simulation.h"

using std::cout;

TestLiveSimulation::TestLiveSimulation() 
	:  _spy(0)
{ }

void TestLiveSimulation::initTestCase() {
	_log = new LogBase;
    _liveSim = new LiveSimulation(this, _log);
	
	QVERIFY(_liveSim);
	QCOMPARE(_liveSim->state(), LiveSimulation::Closed);
	
	QDir dir = UniSim::FileLocations::location(UniSim::FileLocationInfo::Temporary);
	_filePath = dir.absolutePath() + "/test.xml";
	 UniSim::writeStandardTestFile(_filePath);
}

void TestLiveSimulation::cleanupTestCase() {
	delete _log;
}

void TestLiveSimulation::init() {
	QVERIFY(_spy == 0);
    _spy = new QSignalSpy(_liveSim, SIGNAL(stateChanged(int, int)));
}

void TestLiveSimulation::cleanup() {
	_log->clear();
	delete _spy;
	_spy = 0;
}

void TestLiveSimulation::testOpening() {
	open();
}

void TestLiveSimulation::open() {
    _liveSim->open(_filePath);
    QCOMPARE(_log->items()->size(), 1);
	QVERIFY(_log->items()->at(0).title == "Opening file");
	QVERIFY(_log->items()->at(0).text == _filePath);

    waitForStateChange(LiveSimulation::Opening);
    waitForStateChange(LiveSimulation::EndOpening);
    waitForStateChange(LiveSimulation::Expanding);
    waitForStateChange(LiveSimulation::EndExpanding);
    waitForStateChange(LiveSimulation::Initializing);
    waitForStateChange(LiveSimulation::EndInitializing);
    waitForStateChange(LiveSimulation::Ready);
	
    QCOMPARE(_spy->count(), 7);
    QCOMPARE(_liveSim->state(), LiveSimulation::Ready);
}

void TestLiveSimulation::testViewBuilding() {
    _liveSim->writeGraph();
    waitForStateChange(LiveSimulation::ViewBuilding);
    waitForStateChange(LiveSimulation::EndViewBuilding);
    waitForStateChange(LiveSimulation::Ready);

    QCOMPARE(_spy->count(), 3);
    QCOMPARE(_liveSim->state(), LiveSimulation::Ready);
}

void TestLiveSimulation::testRunning() {
    _liveSim->run();
    waitForStateChange(LiveSimulation::Running);
    waitForStateChange(LiveSimulation::EndRunning);
    waitForStateChange(LiveSimulation::Ready);

    QCOMPARE(_spy->count(), 3);
    QCOMPARE(_liveSim->state(), LiveSimulation::Ready);
}

void TestLiveSimulation::testClosing() {
	close();
}

void TestLiveSimulation::close() {
    _liveSim->close();
    waitForStateChange(LiveSimulation::Closing);
    waitForStateChange(LiveSimulation::Closed);
    QCOMPARE(_spy->count(), 2);
    QCOMPARE(_liveSim->state(), LiveSimulation::Closed);
}

void TestLiveSimulation::testOpeningTwice() {
	open();
	_spy->clear();
	_log->clear();
	
    _liveSim->open(_filePath);
	
	QCOMPARE(_log->items()->size(), 2);
	QVERIFY(_log->items()->at(1).title == "Warning");

    QCOMPARE(_liveSim->state(), LiveSimulation::Ready);
}

void TestLiveSimulation::testClosingTwice() {
	close();
	_spy->clear();
	close();
}	

void TestLiveSimulation::testViewBuildingWhenNotReady() {
	QCOMPARE(_liveSim->state(), LiveSimulation::Closed);
	_spy->clear();
	_log->clear();
   _liveSim->writeGraph();

    waitForStateChange(LiveSimulation::ViewBuilding);
    QCOMPARE(_spy->count(), 2);
	QCOMPARE(_liveSim->state(), LiveSimulation::Closed);
}

void TestLiveSimulation::testRunningWhenNotReady() {
	QCOMPARE(_liveSim->state(), LiveSimulation::Closed);
	_spy->clear();
	_log->clear();
    _liveSim->run();
    QCOMPARE(_liveSim->state(), LiveSimulation::Closed);
    QCOMPARE(_spy->count(), 0);
}

void TestLiveSimulation::waitForStateChange(LiveSimulation::State state) {
	const double stepSeconds = 0.25;
        const double maxSeconds = 5;
	double secondsPassed = 0.;
    while (stateMissing(state) && secondsPassed < maxSeconds) {
		secondsPassed += stepSeconds;
		QTest::qWait(int(stepSeconds*1000));
	}
    if (stateMissing(state)) {
        QString msg = "Time out waiting  5 secs. for state: " +
                      LiveSimulation::stateText(state) +
                      "\nCurrent state is " +
                      LiveSimulation::stateText(_liveSim->state());
        QWARN(qPrintable(msg));
		showLog();
	}
}

void TestLiveSimulation::showLog() {
	QWARN(qPrintable("Logged messages: " + _log->allAsText() + "\n"));
}

bool TestLiveSimulation::stateMissing(LiveSimulation::State state) {
	for (int i = 0; i < _spy->count(); ++i) {
		QList<QVariant> arguments =_spy->at(i);
        Q_ASSERT(arguments.size() == 2);
        if (arguments.at(1).toInt() == (int) state)
			return false;
	}
	return true;
}		

void TestLiveSimulation::showSpyStates() {
	QWARN("Spy states:");
	for (int i = 0; i < _spy->count(); ++i) {
		QList<QVariant> arguments =_spy->at(i);
		std::cout << arguments.at(2).toInt() << "\n"; 
	}
}
	

/*
void TestLiveSimulation::testStopCurrentRun()
{
	_liveSim->simulation()->integrator()->changeParameter("numSteps",1000);
    std::cout << "\nbefore " << _liveSim->simulation()->stepCount() << "\n";
	_liveSim->beginRunning();
    waitForStateChange(LiveSimulation::Running);

    QTest::qWait(1);
    _liveSim->simulation()->stopCurrentRun();
 
    waitForStateChange(LiveSimulation::EndRunning);
    std::cout << "after " << _liveSim->simulation()->stepCount() << "\n";
  
}

void TestLiveSimulation::testStopAllRuns()
{
        
}
*/	

