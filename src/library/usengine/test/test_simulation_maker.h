#ifndef TEST_SIMULATION_MAKER_H
#define TEST_SIMULATION_MAKER_H

#include <usbase/test/autotest.h>

namespace UniSim {
	class Simulation;	
}

class TestSimulationMaker : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void testSimulation();
    void testModel();
    void testParameters();
    void cleanupTestCase();
private:
    UniSim::Simulation* createSimulation(QString fileName);
    QString filePath(QString fileName) const;
    UniSim::Simulation *simulation;
};

DECLARE_TEST(TestSimulationMaker)

#endif
