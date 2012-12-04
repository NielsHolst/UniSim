#ifndef UNISIM_TEST_DISTRIBUTED_DELAY_2D_H
#define UNISIM_TEST_DISTRIBUTED_DELAY_2D_H

#include <QPair>
#include <usbase/test/autotest.h>
#include <usengine/simulation.h>
#include "../distributed_delay_2d.h"

using UniSim::DistributedDelay2D;

class TestDistributedDelay2D : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testFlowsUnequal();
    void testFlowsEqual();
    void testFlowsAsymmetric();
    void testFlowsWithGrowth();
private:
    QVector<double> inflow1, inflow2;
    DistributedDelay2D::UpdateParameters up;

    typedef QPair<double,double> Outflows;
    Outflows produceOutflows(DistributedDelay2D::FixedParameters p,
                             double fgr1 = 1.,
                             DistributedDelay2D::Policy policy = DistributedDelay2D::Flexible);
    void show(const UniSim::DistributedDelay2D &dd, DistributedDelay2D::FixedParameters p);
};

DECLARE_TEST(TestDistributedDelay2D)


#endif
