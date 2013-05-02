#ifndef TEST_UNISIM_INFECTION_H
#define TEST_UNISIM_INFECTION_H

#include <usbase/test/autotest.h>

namespace UniSim {
    class Simulation;
}

class TestInfection : public QObject
{
    Q_OBJECT
private slots:
    void cleanup();
    void testRecipe();
    void testTwoParasitoids();

private:
    void createSimulation(QString filename);
    void run(QString filename);
    UniSim::Simulation *sim;
};

DECLARE_TEST(TestInfection)


#endif
