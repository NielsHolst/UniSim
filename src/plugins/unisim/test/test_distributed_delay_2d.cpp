#include <iostream>
#include <usbase/test_num.h>
#include "test_distributed_delay_2d.h"

using namespace UniSim;
using namespace std;

void TestDistributedDelay2D::initTestCase() {
    up.dt1 = up.dt2 = 1;
    up.fgr1 = up.fgr2 = 1;
    up.inflow1 = &inflow1;
    up.inflow2 = &inflow2;
}

void TestDistributedDelay2D::testFlowsUnequal() {
    const DistributedDelay2D::FixedParameters p = {100, 50, 4, 5, 1};
    Outflows out = produceOutflows(p);
    QCOMPARE(out.first + out.second, 100.);
    QVERIFY(out.first < out.second);
}

void TestDistributedDelay2D::testFlowsEqual() {
    const DistributedDelay2D::FixedParameters p = {75, 75, 20, 20, 1};
    Outflows out = produceOutflows(p);
    QCOMPARE(out.first + out.second, 100.);
    QVERIFY(TestNum::eq(out.first, out.second));
}

void TestDistributedDelay2D::testFlowsAsymmetric() {
    const DistributedDelay2D::FixedParameters p = {75, 75, 5, 5, 1};
    Outflows out = produceOutflows(p, 1., DistributedDelay2D::Asymmetric);
    QCOMPARE(out.first + out.second, 100.);
    QVERIFY(out.first > out.second);
}

void TestDistributedDelay2D::testFlowsWithGrowth() {
    const DistributedDelay2D::FixedParameters p = {75, 75, 5, 5, 1};
    Outflows out = produceOutflows(p, 1.5);
    QVERIFY(out.first + out.second > 100.);
    QVERIFY(out.first + out.second < 150.);
    QVERIFY(out.first > out.second);
}


TestDistributedDelay2D::Outflows
TestDistributedDelay2D::produceOutflows(DistributedDelay2D::FixedParameters p,
                                        double fgr1,
                                        DistributedDelay2D::Policy policy)
{
    DistributedDelay2D dd(p,0,policy);
    inflow1.resize(p.k2);
    inflow2.resize(p.k1);
    inflow2[0] = 100;
    DistributedDelay2D::UpdateParameters myUp = up;
    myUp.fgr1 = fgr1;
    double outflow1 = 0, outflow2 = 0;
//    cout << "init\n";
//    show(dd,p);
    for (int i = 0; i < 500; ++i) {
        dd.update(myUp);
        inflow2[0] = 0;
        DistributedDelay2D::State s = dd.state();
        outflow1 += accum(s.outflow1);
        outflow2 += accum(s.outflow2);
//        cout << i << endl;
//        show(dd,p);
    }
    return qMakePair(outflow1, outflow2);
}


void TestDistributedDelay2D::show(const DistributedDelay2D &dd, DistributedDelay2D::FixedParameters p) {
    DistributedDelay2D::State s = dd.state();
    cout << dd.content() << "\t" <<
            s.growthRate << "\t" <<
            accum(s.outflow1) << "\t" <<
            accum(s.outflow2) << "\t" <<
            s.idt << endl;

    const double* v = dd.data();
    int nRows = p.k1, nCols = p.k2;
    for (int i = 0; i < nRows; ++i ) {
        for (int j = 0; j < nCols; ++j ) {
            cout << v[j*p.k1 + i] << "\t";
        }
        cout << endl;
    }
}

