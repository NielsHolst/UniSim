#include <iostream>
#include <QDate>
#include <usbase/exception.h>
#include <usbase/strata.h>
#include <usbase/strata_base.h>
#include "test_strata.h"

using namespace UniSim;

void TestStrata::testAbsoluteInt() {
    Strata<int> strata(30, 20, 4, StrataBase::Absolute);
    QCOMPARE(strata.stratum(0), 10);
    QCOMPARE(strata.stratum(1), 20);
    QCOMPARE(strata.stratum(2), 30);
    QCOMPARE(strata.stratum(3), 40);
    QCOMPARE(strata.stratum(4), 50);
    comp(strata.strata(), strata.samples());

    for (int i = 0; i < 4; ++i)
        QCOMPARE(strata.sample(i), strata.nextSample());
    bool excepted = false;
    try {
        strata.nextSample();
    }
    catch (Exception&) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestStrata::testAbsoluteDouble() {
    Strata<double> strata(0.3, 0.2, 4, StrataBase::Absolute);
    QCOMPARE(strata.stratum(0), 0.1);
    QCOMPARE(strata.stratum(1), 0.2);
    QCOMPARE(strata.stratum(2), 0.3);
    QCOMPARE(strata.stratum(3), 0.4);
    QCOMPARE(strata.stratum(4), 0.5);
    comp(strata.strata(), strata.samples());
}

void TestStrata::testAbsoluteDate() {
    Strata<QDate> strata(QDate(2011,8,14), 12, 4, StrataBase::Absolute);
    QCOMPARE(strata.stratum(0), QDate(2011,8,2));
    QCOMPARE(strata.stratum(1), QDate(2011,8,8));
    QCOMPARE(strata.stratum(2), QDate(2011,8,14));
    QCOMPARE(strata.stratum(3), QDate(2011,8,20));
    QCOMPARE(strata.stratum(4), QDate(2011,8,26));
    comp(strata.strata(), strata.samples());
}

void TestStrata::testRelativeInt() {
    Strata<int> strata(1000, 0.2, 4, StrataBase::Relative);
    QCOMPARE(strata.stratum(0), 800);
    QCOMPARE(strata.stratum(1), 900);
    QCOMPARE(strata.stratum(2), 1000);
    QCOMPARE(strata.stratum(3), 1100);
    QCOMPARE(strata.stratum(4), 1200);
    comp(strata.strata(), strata.samples());
}

void TestStrata::testRelativeDouble() {
    Strata<double> strata(0.5, 0.2, 4, StrataBase::Relative);
    QCOMPARE(strata.stratum(0), 0.40);
    QCOMPARE(strata.stratum(1), 0.45);
    QCOMPARE(strata.stratum(2), 0.50);
    QCOMPARE(strata.stratum(3), 0.55);
    QCOMPARE(strata.stratum(4), 0.60);
    comp(strata.strata(), strata.samples());
}

void TestStrata::testFactorInt() {
    Strata<int> strata(64, 4, 6, StrataBase::Factor);
    QCOMPARE(strata.stratum(0), 16);
    QCOMPARE(strata.stratum(3), 64);
    QCOMPARE(strata.stratum(6), 256);
    comp(strata.strata(), strata.samples());
}

void TestStrata::testFactorDouble() {
    Strata<double> strata(8./27, 4./9, 4, StrataBase::Factor);
    QCOMPARE(strata.stratum(0), 2./3);
    QCOMPARE(strata.stratum(1), 4./9);
    QCOMPARE(strata.stratum(2), 8./27);
    QCOMPARE(strata.stratum(3), 16./81);
    QCOMPARE(strata.stratum(4), 32./243);
    comp(strata.strata(), strata.samples());
}
