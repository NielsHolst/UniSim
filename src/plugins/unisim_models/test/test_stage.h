#ifndef UNISIM_TEST_STAGE_H
#define UNISIM_TEST_STAGE_H

#include <usbase/test/autotest.h>
#include <usengine/simulation.h>

namespace UniSim {
    class Simulation;
}

class TestStage : public QObject
{
    Q_OBJECT

private:
    void execute(QString fileName);
    void executeAndVerify(QString fileBaseName, double sum, double totalGrowth);

private slots:
    void initTestCase();
    void cleanup();
    void testSimulationWithoutGrowth();
    void testSimulationWithGrowth();
    void testSimulationWithGrowthLimited();

private:
    UniSim::Simulation *sim;
};

DECLARE_TEST(TestStage)


#endif
