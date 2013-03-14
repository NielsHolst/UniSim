#include <iostream>
#include <QDate>
#include <QString>
#include <QVariant>
#include "../exception.h"
#include "../identifier.h"
#include "../model.h"
#include "../parameter.h"
#include "../parameter_base.h"
#include "test_parameter.h"

using std::cout;
using namespace UniSim;

void TestParameter::init() {
    model = new UniSim::Model("model",0);
}

void TestParameter::cleanup() {
    delete model;
    model = 0;
}


void TestParameter::testDefaultInt() {
    int i;
    new Parameter<int>("i", &i, 123, model, "desc");
    QCOMPARE(i,123);
    QCOMPARE(model->pullValue<int>("i"), 123);
}

void TestParameter::testChangeInt() {
    int i;
    Parameter<int> *p = new Parameter<int>("i", &i, 123, model, "desc");
    p->setValue(456);
    QCOMPARE(i,456);
    QCOMPARE(model->pullValue<int>("i"), 456);
}

void TestParameter::testSetIntWithStringValue() {
    int i;
    Parameter<int> *p = new Parameter<int>("i", &i, -1, model, "desc");
    try {
        p->setValueFromString("123");
        QCOMPARE(i,123);
        QCOMPARE(model->pullValue<int>("i"), 123);
    }
    catch (const Exception &ex) {
        QFAIL(qPrintable(ex.message()));
    }
}

void TestParameter::testDefaultBool() {
    bool b;
    new Parameter<bool>("b", &b, false, model, "desc");
    QCOMPARE(b, false);
    QCOMPARE(model->pullValue<bool>("b"), false);
}

void TestParameter::testChangeBool() {
    bool b;
    Parameter<bool> *p = new Parameter<bool>("b", &b, false, model, "desc");
    p->setValue(true);
    QCOMPARE(b, true);
    QCOMPARE(model->pullValue<bool>("b"), true);
}

void TestParameter::testSetBoolWithStringValue() {
    bool b;
    Parameter<bool> *p = new Parameter<bool>("b", &b, false, model, "desc");
    try {
        p->setValueFromString("yes");
        QCOMPARE(b, true);
        QCOMPARE(model->pullValue<bool>("b"), true);

        p->setValueFromString("NO");
        QCOMPARE(b, false);
        QCOMPARE(model->pullValue<bool>("b"), false);
    }
    catch (const Exception &ex) {
        QFAIL(qPrintable(ex.message()));
    }
}

void TestParameter::testDefaultString() {
    QString s;
    new Parameter<QString>("s", &s, QString("top"), model, "desc");
    QCOMPARE(s, QString("top"));
    QCOMPARE(model->pullValue<QString>("s"), QString("top"));
}

void TestParameter::testChangeString() {
    QString s;
    Parameter<QString> *p = new Parameter<QString>("s", &s, QString("top#"), model, "desc");
    p->setValue("bottom");
    QCOMPARE(s, QString("bottom"));
    QCOMPARE(model->pullValue<QString>("s"), QString("bottom"));
}

void TestParameter::testSetStringWithStringValue() {
    QString s;
    Parameter<QString> *p = new Parameter<QString>("s", &s, QString(), model, "desc");
    try {
        p->setValueFromString("top");
        QCOMPARE(s, QString("top"));
        QCOMPARE(model->pullValue<QString>("s"), QString("top"));
    }
    catch (const Exception &ex) {
        QFAIL(qPrintable(ex.message()));
    }
}

void TestParameter::testDefaultDate() {
    QDate date;
    new Parameter<QDate>("date", &date, QDate(2008, 12, 23), model, "desc");
    QCOMPARE(date, QDate(2008, 12, 23));
    QCOMPARE(model->pullValue<QDate>("date"), QDate(2008, 12, 23));
}

void TestParameter::testChangeDate() {
    QDate date;
    Parameter<QDate> *p = new Parameter<QDate>("date", &date, QDate(2008, 12, 23), model, "desc");
    p->setValue(QDate(1972, 6, 15));
    QCOMPARE(date, QDate(1972, 6, 15));
    QCOMPARE(model->pullValue<QDate>("date"), QDate(1972, 6, 15));
}

void TestParameter::testSetDateWithStringValue() {
    QDate date = QDate(2008, 12, 23);
    Parameter<QDate> *p = new Parameter<QDate>("date", &date, QDate(), model, "desc");
    try {
        p->setValueFromString("23/12/2008");
        QCOMPARE(date, QDate(2008, 12, 23));
        QCOMPARE(model->pullValue<QDate>("date"), QDate(2008, 12, 23));
    }
    catch (const Exception &ex) {
        QFAIL(qPrintable(ex.message()));
    }
}

void TestParameter::testIntToDouble() {
    int i;
    new Parameter<int>("i", &i, 123, model, "desc");
    bool excepted = false;
    try {
        model->pullValue<double>("i");
    }
    catch (const Exception &ex) {
            excepted = true;
    }
    QVERIFY(excepted);
}

void TestParameter::testDoubleToInt() {
    double x;
    new Parameter<double>("x", &x, 17.4, model, "desc");
    bool excepted = false;
    try {
        model->pullValue<int>("x");
    }
    catch (const Exception &ex) {
            excepted = true;
    }
    QVERIFY(excepted);
}

void TestParameter::testIntToDate() {
    int i;
    new Parameter<int>("i", &i, 123, model, "desc");
    bool excepted = false;
    try {
        model->pullValue<QDate>("i");
    }
    catch (const Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestParameter::testMissing() {
    QVERIFY(model);
    bool excepted = false;
    try {
        QCOMPARE(model->pullValue<int>("j"), 123);
    }
    catch (const Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestParameter::testBaseClass() {
    int i;
    Parameter<int> *p = new Parameter<int>("i", &i, 123, model, "desc");
    // Base: ParameterBase
    ParameterBase *q = p;
    q->setValueFromString("456");
    QCOMPARE(model->pullValue<int>("i"), 456);

    // Base: Variable<T>
    Variable<int> *r = p;
    QCOMPARE(r->toString(), QString("456"));

    // Base: VariableBase
    VariableBase *s = p;
    QCOMPARE(s->toString(), QString("456"));
}

