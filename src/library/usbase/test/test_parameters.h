#ifndef UniSim_TEST_PARAMETERS_H
#define UniSim_TEST_PARAMETERS_H

#include <QObject>
#include <usbase/test/autotest.h>
	
class TestParameters : public QObject
{
    Q_OBJECT
    
private slots:
    void testSetIntWithStringValue();
    void testSetStringWithStringValue();
    void testSetDateWithStringValue();
    void testSetBoolWithStringValue();
	
    void testSetIntWithoutStringValue();
    void testSetStringWithoutStringValue();
    void testSetDateWithoutStringValue();
    void testSetBoolWithoutStringValue();
    
	void testSetSameTwice();    
	void testConflictingStringValueType();
	
    void testChangeInt();
    void testChangeString();
    void testChangeDate();
    void testChangeBool();

    
};

DECLARE_TEST(TestParameters)

#endif 
