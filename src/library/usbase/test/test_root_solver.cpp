#include <usbase/exception.h>
#include "test_root_solver.h"

using namespace UniSim;

namespace {

    double fn1 (double x, void *parameters) {
        TestRootSolver::Parameters *p = (TestRootSolver::Parameters*) parameters;
        return (p->a*x + p->b)*x + p->c;
    }
}

void TestRootSolver::init() {
    p.a = 1;
    p.b = 0;
    p.c = -5;
    problem.x.lower = 0.;
    problem.x.upper = 6.;
    problem.function = fn1;
    problem.parameters = &p;
    problem.maxIterations = 100;
    problem.epsAbs = 0.001;
    problem.epsRel = 0;
}

void TestRootSolver::testConvergence() {
    RootSolver::Solution sol;
    try {
        RootSolver rs(problem);
        rs.solve();
        sol = rs.solution();
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception: " + ex.message();
        QFAIL(qPrintable(msg));
    }
    QVERIFY(sol.converged);
    QVERIFY(fabs(sol.root - sqrt(5)) < problem.epsAbs);
}
	
void TestRootSolver::testNonConvergence() {
    problem.maxIterations = 2;
    RootSolver::Solution sol;
    try {
        RootSolver rs(problem);
        rs.solve();
        sol = rs.solution();
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception: " + ex.message();
        QFAIL(qPrintable(msg));
    }
    QVERIFY(!sol.converged);
    QCOMPARE(sol.iterations, problem.maxIterations);
}
