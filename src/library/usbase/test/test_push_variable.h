#ifndef UNISIM_TEST_PUSH_VARIABLE_H
#define UNISIM_TEST_PUSH_VARIABLE_H

#include <QObject>
#include <usbase/test/autotest.h>
	
namespace UniSim {
    class Model;
}

class TestPushVariable : public QObject
{
    Q_OBJECT
    
private slots:
    void init();
    void cleanup();

    void testInt();
    void testBool();
    void testDate();

private:
    UniSim::Model *model;
};

DECLARE_TEST(TestPushVariable)

#endif 
