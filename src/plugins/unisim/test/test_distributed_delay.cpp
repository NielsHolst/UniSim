#include <iostream>
#include <usbase/test_num.h>
#include "../distributed_delay.h"
#include "test_distributed_delay.h"

using namespace UniSim;

namespace {
    const DistributedDelay::Parameters p = {50, 30, 1};
    const double inflow = 100, dt = 1, fgr = 7;
    const double demand = 1.005203254;
}

void TestDistributedDelay::testBracket() {
    verifyGrowth(0.999);
    verifyGrowth(0.5);
    verifyGrowth(0.001);
}

void TestDistributedDelay::verifyGrowth(double sdRatio) {
    DistributedDelay dd(p, 0);
    DistributedDelay::Brackets b;
    b = dd.bracket(inflow, dt, fgr, sdRatio);
    QVERIFY( TestNum::eq(growth(b.fgr1), demand) );
    QVERIFY( TestNum::geZero(growth(b.fgr2)) );
    QVERIFY( growth(b.fgr2) <= sdRatio*demand);
}

double TestDistributedDelay::growth(double fgr) {
    DistributedDelay dd(p, 0);
    dd.update(inflow, dt, fgr);
    return dd.state().growthRate;
}

double TestDistributedDelay::growthZeroInflow(double fgr) {
    DistributedDelay dd(p, 0);
    dd.update(0, dt, fgr);
    return dd.state().growthRate;
}

void TestDistributedDelay::testFindFgrBig() {
    DistributedDelay dd(p, 0);
    double fit = dd.findFgr(inflow, dt, fgr, 0.999);
    QVERIFY( TestNum::eq(growth(fit), 0.999*demand) );
}

void TestDistributedDelay::testFindFgrSmall() {
    DistributedDelay dd(p, 0);
    double fit = dd.findFgr(inflow, dt, fgr, 0.001);
    QVERIFY( TestNum::eq(growth(fit), 0.001*demand) );
}

void TestDistributedDelay::testFindFgrAtDemand() {
    DistributedDelay dd(p, 0);
    double fit = dd.findFgr(inflow, dt, fgr, 1);
    QCOMPARE(fit, fgr);
}

void TestDistributedDelay::testFindFgrZeroSupply(){
    DistributedDelay dd(p, 0);
    double fit = dd.findFgr(inflow, dt, fgr, 0);
    QCOMPARE(fit, 1.);
}

void TestDistributedDelay::testFindFgrZeroInflow() {
    DistributedDelay dd(p, 0);
    double fit = dd.findFgr(0., dt, fgr, 1);
    QVERIFY( TestNum::eqZero(growthZeroInflow(fit)) );
}
