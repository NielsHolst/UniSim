#ifndef UNISIM_TEST_MINIMISER_H
#define UNISIM_TEST_MINIMISER_H

#include <QObject>
#include <QtTest/QtTest>
#include <usbase/test/autotest.h>

	
class TestMinimiser : public QObject
{
    Q_OBJECT
    
private slots:
    void testConvergence();
    void testConvergenceWithValues();
};

DECLARE_TEST(TestMinimiser)

#endif 
