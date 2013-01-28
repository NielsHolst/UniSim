#ifndef TEST_UNISIM_PREDATION_H
#define TEST_UNISIM_PREDATION_H

#include <usbase/test/autotest.h>

namespace UniSim {
    class Simulation;
}

class TestPredation : public QObject
{
    Q_OBJECT
private slots:
    void cleanup();
    void testRecipe();

private:
    void createSimulation(QString filename);
    UniSim::Simulation *sim;
};

DECLARE_TEST(TestPredation)


#endif
