#include <usbase/exception.h>
#include <usbase/reference.h>
#include "test_reference.h"

using namespace UniSim;

void TestReference::testSplit() {
    Reference ref{"dogs/female[size]"};
    QString path, name;
    try {
        path = ref.path();
        name = ref.name();
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.message()));
    }
    QCOMPARE(path, QString("dogs/female"));
    QCOMPARE(name, QString("size"));
}

void TestReference::testMissingLeftBracket() {
    bool excepted{false};
    try {
        Reference{"femalesize]"};
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestReference::testMissingRightBracket() {
    bool excepted{false};
    try {
        Reference{"female[size"};
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestReference::testMissplacedLeftBracket() {
    bool excepted{false};
    try {
        Reference{"[femalesize]"};
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestReference::testMissingName() {
    bool excepted{false};
    try {
        Reference{"female[]"};
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
}
