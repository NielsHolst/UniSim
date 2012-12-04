#include <iostream>
#include <QDate>
#include <QString>
#include <QVariant>
#include "../exception.h"
#include "../identifier.h"
#include "../model.h"
#include "../parameter_vector.h"
#include "test_parameter_vector.h"

using namespace UniSim;

void TestParameterVector::init() {
    model = new UniSim::Model("model",0);
    u << 7 << 9 << 13;
    w << 2 << 4 << 8;
}

void TestParameterVector::cleanup() {
    delete model;
    model = 0;
    u.clear();
    w.clear();
}

void TestParameterVector::testSetValue() {
    ParameterVector<int> *v = new ParameterVector<int>("aVector", &u, model, "desc");
    QCOMPARE(v->value(), u);
    v->setValue(w);
    QCOMPARE(v->value(), w);
}

void TestParameterVector::testSetValueFromString() {
    ParameterVector<int> *v = new ParameterVector<int>("aVector", &u, model, "desc");
    bool excepted = false;
    try {
        v->setValueFromString("1 2 3");
    }
    catch (Exception &) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestParameterVector::testRedirection() {
    ParameterVector<int> *v = new ParameterVector<int>("aVector", &u, model, "desc");
    v->redirectValuePtr(&w);
    QCOMPARE(v->value(), u);
    v->followRedirection();
    QCOMPARE(v->value(), w);
}

void TestParameterVector::testToVariant() {
    ParameterVector<int> *v = new ParameterVector<int>("aVector", &u, model, "desc");
    QCOMPARE(v->toVariant(), QVariant(29));
}

void TestParameterVector::testToString() {
    ParameterVector<int> *v = new ParameterVector<int>("aVector", &u, model, "desc");
    QCOMPARE(v->toString(), QString("29"));
}

void TestParameterVector::testPullValueVector() {
    new ParameterVector<int>("aVector", &u, model, "desc");
    QCOMPARE(model->pullValueVector<int>("aVector"), u);
}

void TestParameterVector::testPullValueVectorPtr() {
    new ParameterVector<int>("aVector", &u, model, "desc");
    QCOMPARE(*( model->pullValueVectorPtr<int>("aVector") ), u);
}

void TestParameterVector::testPushValueVector() {
    new ParameterVector<int>("aVector", &u, model, "desc");
    model->pushValueVector<int>("aVector", w);
    QCOMPARE(model->pullValueVector<int>("aVector"), w);
}

