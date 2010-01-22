#ifndef TEST_SIMULATION_H
#define TEST_SIMULATION_H

#include <usbase/test/autotest.h>

namespace UniSim {
	class Simulation;	
}

class TestSimulation : public QObject
{
    Q_OBJECT
private slots:
	void initTestCase();
    void init();
    void testInitialize();
	void testFindModels();
	void testFindOutputs();
	void testExecute();
    void cleanupTestCase();
private:
    UniSim::Simulation *_simulation;
};

DECLARE_TEST(TestSimulation)

#endif
