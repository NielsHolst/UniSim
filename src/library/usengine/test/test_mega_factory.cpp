#include <iostream>
#include <QObject>
#include <usbase/exception.h>
#include <usbase/model.h>
#include <usbase/output.h>
#include "../mega_factory.h"
#include "test_mega_factory.h"

using namespace UniSim;

void TestMegaFactory::testCreateModelExists() {
    Model *larva = MegaFactory::create<Model>("UniSim::Stage", "larva", 0);
    QVERIFY(larva);
    QCOMPARE(larva->id(), Identifier("larva"));
    QCOMPARE(larva->objectName(), QString("larva"));
    QVERIFY(larva->peekOneChild<ParameterBase*>("k"));

    Model *time = MegaFactory::create<Model>("UniSim::Days", "time", larva);
    QVERIFY(time);
    QVERIFY(time->parent()==larva);

    delete larva;
}

void TestMegaFactory::testCreateModelNoClass() {
    try {
        MegaFactory::create<Model>("UniSim::NotAClass", "larva", 0);
        QFAIL("Exception expected");
    }
    catch (Exception &ex) {
        std::cout << qPrintable(ex.message());
    }
}

void TestMegaFactory::testCreateModelWrongClass() {
    try {
        MegaFactory::create<Output>("UniSim::StageClass", "larva", 0);
        QFAIL("Exception expected");
    }
    catch (Exception &ex) {
        std::cout << qPrintable(ex.message());
    }
}
 
