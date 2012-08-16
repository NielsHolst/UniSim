#ifndef UNISIM_TEST_ROOT_SOLVER_H
#define UNISIM_TEST_ROOT_SOLVER_H

#include <QObject>
#include <QtTest/QtTest>
#include <usbase/test/autotest.h>
#include <usbase/root_solver.h>

	
class TestRootSolver : public QObject
{
    Q_OBJECT
public:
    struct Parameters {
        double a, b, c;
    } p;

private slots:
    void init();
    void testConvergence();
    void testNonConvergence();

private:
    UniSim::RootSolver::Problem problem;
};

DECLARE_TEST(TestRootSolver)

#endif 
