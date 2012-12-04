#ifndef UNISIM_TEST_PARAMETER_VECTOR_H
#define UNISIM_TEST_PARAMETER_VECTOR_H

#include <QObject>
#include <QVector>
#include <usbase/test/autotest.h>
	
namespace UniSim {
    class Model;
}

class TestParameterVector : public QObject
{
    Q_OBJECT
    
private slots:
    void init();
    void cleanup();

    void testSetValue();
    void testSetValueFromString();
    void testRedirection();
    void testToVariant();
    void testToString();
    void testPullValueVector();
    void testPullValueVectorPtr();
    void testPushValueVector();

private:
    UniSim::Model *model;
    QVector<int> u, w;
};

DECLARE_TEST(TestParameterVector)

#endif 
