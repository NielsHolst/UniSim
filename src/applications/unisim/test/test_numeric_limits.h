#ifndef TEST_NUMERIC_LIMITS_H
#define TEST_NUMERIC_LIMITS_H

#include <QObject>

#include <usbase/test/autotest.h>


class TestNumericLimits : public QObject
{
    Q_OBJECT
private slots:
    void testInfinity();
};

DECLARE_TEST(TestNumericLimits)


#endif
