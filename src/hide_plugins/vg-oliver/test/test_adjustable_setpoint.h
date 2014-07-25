#ifndef TEST_ADJUSTABLE_SETPOINT_H
#define TEST_ADJUSTABLE_SETPOINT_H

#include <usbase/test/autotest.h>

class TestAdjustableSetpoint : public QObject
{
    Q_OBJECT
private slots:
    void testIncrement();
    void testDecrement();
};

DECLARE_TEST(TestAdjustableSetpoint)


#endif
