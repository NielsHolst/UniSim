#ifndef TEST_FUNCTIONAL_RESPONSE_G_B_H
#define TEST_FUNCTIONAL_RESPONSE_G_B_H

#include <usbase/test/autotest.h>

namespace UniSim {
    class Model;
    class Simulation;
}

class TestFunctionalResponseGB : public QObject
{
    Q_OBJECT
private slots:
    void testPredation();
    void testInfection();
};

DECLARE_TEST(TestFunctionalResponseGB)


#endif
