#ifndef TEST_SIMULATION_TRICKLE_H
#define TEST_SIMULATION_TRICKLE_H

#include <usbase/test/autotest.h>

namespace UniSim {
    class Simulation;
    class Integrator;
}
class TrickleBox;

class TestSimulationTrickle : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void testExecute();
    void cleanupTestCase();
private:
    void executeAndTest(int steps, int check0, int check1, int check2);
    void textBox(int boxNumber, int contents);
    UniSim::Simulation *simulation;
    UniSim::Integrator *integrator;
    TrickleBox	 *boxes[3];

};

DECLARE_TEST(TestSimulationTrickle)

#endif // TEST_SIMULATION_TRICKLE_H
