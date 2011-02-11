#ifndef UNISIM_TEST_TIME_H
#define UNISIM_TEST_TIME_H

#include <QObject>
#include <QtTest/QtTest>
#include <usbase/test/autotest.h>

	
class TestTime : public QObject
{
    Q_OBJECT
    
private slots:
    void testConversion();
    void testAddSeconds();
    void testAddMinutes();
    void testAddHours();
    void testAddDays();
    void testAddYears();
};

DECLARE_TEST(TestTime)

#endif 
