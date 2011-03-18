#ifndef TEST_UNISIM_FIXED_H
#define TEST_UNISIM_FIXED_H

#include <usbase/test/autotest.h>

namespace UniSim {
    class Model;
    class Simulation;
}

class TestFixed : public QObject
{
    Q_OBJECT
private slots:
    void cleanup();
    void testPullVariables();

private:
    void createSimulation(QString filename);

    UniSim::Simulation *sim;
};

DECLARE_TEST(TestFixed)


#endif
