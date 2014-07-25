#ifndef TEST_STEPS_TIME_INTERVAL_H
#define TEST_STEPS_TIME_INTERVAL_H

#include <usbase/test/autotest.h>

namespace UniSim {
    class Model;
    class Simulation;
}

class TestStepsTimeInterval : public QObject
{
    Q_OBJECT
private slots:
    void cleanup();
    void testSum();

private:
    void createSimulation(QString filename);

    UniSim::Simulation *sim;
};

DECLARE_TEST(TestStepsTimeInterval)


#endif
