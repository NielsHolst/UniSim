#include <iostream>
#include <QDate>
#include <QString>
#include <QVariant>
#include "../exception.h"
#include "../identifier.h"
#include "../model.h"
#include "../parameter.h"
#include "test_push_variable.h"

using namespace UniSim;

void TestPushVariable::init() {
    model = new UniSim::Model("model",0);
}

void TestPushVariable::cleanup() {
    delete model;
    model = 0;
}


void TestPushVariable::testInt() {
    int i;
    Parameter<int> *var = new Parameter<int>("i", &i, 0, model, "desc");
    var->setValue(123);
    QCOMPARE(i,123);

}

void TestPushVariable::testBool() {
    bool i;
    Parameter<bool> *var = new Parameter<bool>("i", &i, true, model, "desc");

    var->setValue(true);
    QCOMPARE(i,true);
    var->setValue(false);
    QCOMPARE(i,false);
}

void TestPushVariable::testDate() {
    QDate i;
    Parameter<QDate> *var = new Parameter<QDate>("i", &i, QDate(), model, "desc");
    var->setValue(QDate(2010,12,24));
    QCOMPARE(i,QDate(2010,12,24));
}



