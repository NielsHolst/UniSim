#ifndef TEST_UNISIM_RANGE_H
#define TEST_UNISIM_RANGE_H

#include <usbase/test/autotest.h>

namespace UniSim {
    class Model;
    class Simulation;
}

class TestRange : public QObject
{
    Q_OBJECT
private slots:
    void cleanup();
    void testLinear();
    void testLog10();

private:
    void createSimulation(QString filename);

    UniSim::Simulation *sim;
};

DECLARE_TEST(TestRange)


#endif
