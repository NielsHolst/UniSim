#include <iostream>
#include "../identifier.h"
#include "test_identifier.h"

using namespace UniSim;
	
void TestIdentifier::testConstruction() {
    QString name = " A B   c  ";
    Identifier id1(name);

    QCOMPARE(id1.key(), QString("a_b_c"));
    QCOMPARE(id1.label(), name);
}

void TestIdentifier::testEquality() {
    Identifier id1("abc"), id2("abc");

    QVERIFY(id1.equals("abc"));
    QVERIFY(id1.equals(" abc "));
    QVERIFY(!id1.equals("xyz"));
    QVERIFY(id1==id2);
}

void TestIdentifier::testInequality() {
    Identifier id1("abc"), id2("abc"), id3("def");
    QVERIFY(!(id1 < id2));
    QVERIFY(id1 < id3);
}

void TestIdentifier::testIdentifiers() {
    Identifiers ids;
    ids << Identifier("cat") << Identifier("dog");
    QVERIFY(ids.contains(Identifier("cat")));
}



