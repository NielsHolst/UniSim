#include <iostream>
#include <usbase/exception.h>
#include <usbase/minimiser.h>
#include <usbase/utilities.h>
#include "test_minimiser.h"

using namespace UniSim;

namespace {
    double fn1 (double x, void*) {
        return cos(x) + 1.0;
    }
}


void TestMinimiser::testConvergence() {
    Minimiser::Problem problem;
    problem.x.lower = 0.;
    problem.x.upper = 6.;
    problem.x.minimum = 2.;
    problem.function = fn1;
    problem.maxIterations = 100;
    problem.epsAbs = 0.001;
    problem.epsRel = 0.;

    Minimiser::Solution sol;
    try {
        Minimiser mi(problem);
        mi.solve();
        sol = mi.solution();
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception: " + ex.message();
        QFAIL(qPrintable(msg));
    }
    QVERIFY(sol.converged);
    QVERIFY(fabs(sol.x.minimum - PI) < problem.epsAbs);

    problem.maxIterations = 2;
    try {
        Minimiser mi(problem);
        mi.solve();
        sol = mi.solution();
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception: " + ex.message();
        QFAIL(qPrintable(msg));
    }
    QVERIFY(!sol.converged);
    QCOMPARE(sol.iterations, problem.maxIterations);
}
	

void TestMinimiser::testConvergenceWithValues() {
    Minimiser::Problem problem;
    problem.x.lower = 0.;
    problem.x.upper = 6.;
    problem.x.minimum = 2.;
    problem.function = fn1;
    problem.maxIterations = 100;
    problem.epsAbs = 0.001;
    problem.epsRel = 0.;

    Minimiser::Values values;
    values.f.lower = fn1(problem.x.lower, 0);
    values.f.upper = fn1(problem.x.upper, 0);
    values.f.minimum = fn1(problem.x.minimum, 0);

    Minimiser::Solution sol;
    try {
        Minimiser mi(problem, values);
        mi.solve();
        sol = mi.solution();
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception: " + ex.message();
        QFAIL(qPrintable(msg));
    }
    QVERIFY(sol.converged);
    QVERIFY(fabs(sol.x.minimum - PI) < problem.epsAbs);
}
