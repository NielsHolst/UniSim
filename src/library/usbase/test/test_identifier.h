#ifndef UniSim_TEST_IDENTIFIER_H
#define UniSim_TEST_IDENTIFIER_H

#include <QObject>
#include <QtTest/QtTest>
#include <usbase/test/autotest.h>

	
class TestIdentifier : public QObject
{
    Q_OBJECT
    
private slots:
    void testConstruction();
    void testEquality();
    void testInequality();
    void testIdentifiers();
};

DECLARE_TEST(TestIdentifier)

#endif 
