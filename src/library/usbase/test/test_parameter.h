#ifndef UniSim_TEST_PARAMETERS_H
#define UniSim_TEST_PARAMETERS_H

#include <QObject>
#include <usbase/test/autotest.h>
	
namespace UniSim {
    class Model;
}

class TestParameter : public QObject
{
    Q_OBJECT
    
private slots:
    void init();
    void cleanup();

    void testNameMacro();

    void testDefaultInt();
    void testChangeInt();
    void testSetIntWithStringValue();

    void testDefaultString();
    void testChangeString();
    void testSetStringWithStringValue();

    void testDefaultDate();
    void testChangeDate();
    void testSetDateWithStringValue();

    void testDefaultBool();
    void testChangeBool();
    void testSetBoolWithStringValue();
	
    void testIntToDouble();
    void testDoubleToInt();
    void testIntToDate();

    void testMissing();

    void testBaseClass();
private:
    UniSim::Model *model;
};

DECLARE_TEST(TestParameter)

#endif 
