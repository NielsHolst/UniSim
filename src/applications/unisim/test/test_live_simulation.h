#ifndef TEST_LIVE_SIMULATION_H
#define TEST_LIVE_SIMULATION_H

#include <QString>
#include <QtTest/QtTest>
#include <usbase/test/autotest.h>
#include "../live_simulation.h"

class LogBase;
class QSignalSpy;

class TestLiveSimulation : public QObject
{
    Q_OBJECT
public:
	TestLiveSimulation();

private slots:
	void initTestCase();
    void cleanupTestCase();
    
    void init();
    void cleanup();

    void testOpening();
    void testViewBuilding();
    void testRunning();
    void testClosing();
    
    void testOpeningTwice();
    void testClosingTwice();
	void testViewBuildingWhenNotReady();
    void testRunningWhenNotReady();
    
private:
	void open();
	void close();
	void showLog();
	void showSpyStates();
    void waitForStateChange(LiveSimulation::State state);
	bool stateMissing(LiveSimulation::State state);
	
	LogBase *_log;
	LiveSimulation *_liveSim;
	QString _filePath;
	QSignalSpy *_spy;
};

DECLARE_TEST(TestLiveSimulation)

#endif
