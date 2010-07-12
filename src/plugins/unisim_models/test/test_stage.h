#ifndef UNISIM_TEST_STAGE_H
#define UNISIM_TEST_STAGE_H

#include <usbase/test/autotest.h>

namespace UniSim {
    class Model;
}

class TestStage : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testAgeClasses();

private:
    UniSim::Model *model;
};

DECLARE_TEST(TestStage)


#endif
