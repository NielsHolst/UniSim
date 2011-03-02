#include <iostream>
#include "../optimize_allocation.h"
#include "test_plant.h"

using namespace intercom::OptimizeAllocation;
using std::cout;

void TestPlant::testOptimizeAllocationMixed(){
    Solution solution;
    SolutionEntry
            v0(-2.4, 0),
            v1( 1.6, 1),
            v2(-0.4, 2),
            v3( 2.2, 3);
    solution.append(v0);
    solution.append(v1);
    solution.append(v2);
    solution.append(v3);

    optimize(solution);

    QCOMPARE(solution[3].second, 3);
    QCOMPARE(solution[2].second, 1);

    QVERIFY(solution[1].second==0 || solution[1].second==2);
    QVERIFY(solution[0].second==0 || solution[0].second==2);
    QVERIFY(solution[1].second != solution[0].second);

    QCOMPARE(solution[3].first, 0.8);
    QCOMPARE(solution[2].first, 0.2);
    QCOMPARE(solution[1].first, 0.0);
    QCOMPARE(solution[0].first, 0.0);
}

void TestPlant::testOptimizeAllocationAllPositive(){
    Solution solution;
    SolutionEntry
            v0(0.4, 0),
            v1(0.1, 1),
            v2(0.5, 2);
    solution.append(v0);
    solution.append(v1);
    solution.append(v2);

    optimize(solution);

    QCOMPARE(solution[0].second, 1);
    QCOMPARE(solution[1].second, 0);

    QCOMPARE(solution[0].first, 0.1);
    QCOMPARE(solution[1].first, 0.4);
    QCOMPARE(solution[2].first, 0.5);
}
