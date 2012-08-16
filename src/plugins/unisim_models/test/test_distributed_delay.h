#ifndef UNISIM_TEST_DISTRIBUTED_DELAY_H
#define UNISIM_TEST_DISTRIBUTED_DELAY_H

#include <usbase/test/autotest.h>
#include <usengine/simulation.h>

namespace UniSim {
    class Simulation;
}

class TestDistributedDelay : public QObject
{
    Q_OBJECT

private:
    double growth(double fgr);
    double growthZeroInflow(double fgr);
    void verifyGrowth(double sdRatio);
private slots:
    void testBracket();

    void testFindFgrBig();
    void testFindFgrSmall();
    void testFindFgrAtDemand();
    void testFindFgrZeroSupply();
    void testFindFgrZeroInflow();
};

DECLARE_TEST(TestDistributedDelay)


#endif
