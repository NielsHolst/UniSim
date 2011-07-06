#include <iostream>
#include <QDate>
#include <QString>
#include <QVariant>
#include "../exception.h"
#include "../identifier.h"
#include "../model.h"
#include "../push_variable.h"
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
    PushVariable<int> *var = new PushVariable<int>("i", &i, model, "desc");
    var->setValue(123);
    QCOMPARE(i,123);

    var->addValue(45);
    QCOMPARE(i,123+45);

    var->resetValue();
    QCOMPARE(i,0);
}

void TestPushVariable::testBool() {
    bool i;
    PushVariable<bool> *var = new PushVariable<bool>("i", &i, model, "desc");

    var->setValue(true);
    QCOMPARE(i,true);
    var->setValue(false);
    QCOMPARE(i,false);

    var->addValue(false);
    QCOMPARE(i,false);
    var->addValue(true);
    QCOMPARE(i,true);

    var->resetValue();
    QCOMPARE(i,false);
}

void TestPushVariable::testDate() {
    QDate i;
    PushVariable<QDate> *var = new PushVariable<QDate>("i", &i, model, "desc");
    var->setValue(QDate(2010,12,24));
    QCOMPARE(i,QDate(2010,12,24));

    var->addValue(QDate(2010,12,31));
    QCOMPARE(i,QDate(2010,12,31));

    var->resetValue();
    QVERIFY(i.isNull());
}



