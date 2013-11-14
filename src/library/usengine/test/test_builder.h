#ifndef UNISIM_TEST_BUILDER_H
#define UNISIM_TEST_BUILDER_H

#include <usbase/test/autotest.h>

class TestBuilder : public QObject
{
    Q_OBJECT
private slots:
    void testSimulation();
    void testController();
    void testModel();
    void testBare();
    void testParameter();
    void testBegin();
    void testEnd();

    //void testSmall();
};

DECLARE_TEST(TestBuilder)


#endif
