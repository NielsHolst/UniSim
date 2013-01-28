#include <iostream>
#include <usbase/exception.h>
#include <usbase/test_num.h>
#include "../forage.h"
#include "test_forage.h"

using namespace UniSim;

void TestForage::initTestCase() {
    p1 << 0.5;
    p2 << 0.5 << 0.4;
    p3 << 0.5 << 0.4 << 0.3;
}

namespace {
    template <class T>
    void show(const char *s, QVector<T> v) {
        std::cout << s << " =\n";
        for (int i=0; i < v.size(); ++i)
            std::cout << v[i] << "\n";
        std::cout << "\n";
    }

    template <class T>
    void show(const char *s, Matrix<T> v) {
        std::cout << s << " =\n";
        for (int i=0; i < v.numRows(); ++i) {
            for (int j=0; j < v.numColumns(); ++j) {
                std::cout << v.at(i,j) << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
}

void TestForage::testParts() {
    QVector<double> parts[3], expected[3];
    parts[0] = Forage::parts(p1);
    parts[1] = Forage::parts(p2);
    parts[2] = Forage::parts(p3);
    expected[0] << 0.5;
    expected[1] << 0.3 << 0.2 << 0.2;
    expected[2] << 0.21 << 0.09 << 0.14 << 0.06 << 0.14 << 0.06 << 0.09;
    QVERIFY(compareVectors(parts[0], expected[0]));
    QVERIFY(compareVectors(parts[1], expected[1]));
    QVERIFY(compareVectors(parts[2], expected[2]));
}

void TestForage::testFirstLevel() {
    QVector<int> v[3], expected[3];
    v[0] = Forage::firstLevel(1);
    v[1] = Forage::firstLevel(2);
    v[2] = Forage::firstLevel(3);
    expected[0] << 0;
    expected[1] << 1 << 2 ;
    expected[2] << 1 << 3 << 5 << 6;
    QCOMPARE(v[0], expected[0]);
    QCOMPARE(v[1], expected[1]);
    QCOMPARE(v[2], expected[2]);
}

void TestForage::testFinalLevel() {
    QVector<int> v[3], expected[3];
    v[0] = Forage::finalLevel(1,3);
    v[1] = Forage::finalLevel(2,3);
    v[2] = Forage::finalLevel(3,3);
    expected[0] << 0 << 1 << 2 << 3;
    expected[1] << 2 << 3 << 4 << 5;
    expected[2] << 1 << 3 << 5 << 6;
    QCOMPARE(v[0], expected[0]);
    QCOMPARE(v[1], expected[1]);
    QCOMPARE(v[2], expected[2]);
}

void TestForage::testOwners() {
    QVector<double> expected[3];
    Matrix<double> m[3], expectedM[3];
    m[0] = Forage::owners(1);
    m[1] = Forage::owners(2);
    m[2] = Forage::owners(3);
    expected[0] << 1;
    expected[1] << 1 << 1 << 0
                << 0 << 1 << 1;
    expected[2] << 1 << 1 << 1 << 1 << 0 << 0 << 0
                << 0 << 0 << 1 << 1 << 1 << 1 << 0
                << 0 << 1 << 0 << 1 << 0 << 1 << 1;
    expectedM[0].fill(1, 1, expected[0]);
    expectedM[1].fill(2, 3, expected[1]);
    expectedM[2].fill(3, 7, expected[2]);
    QCOMPARE(m[0], expectedM[0]);
    QCOMPARE(m[1], expectedM[1]);
    QCOMPARE(m[2], expectedM[2]);
}

void TestForage::testForage() {
    QVector<double> prop[3], expected[3], result[3];
    prop[0] << 0.5;
    prop[1] << 0.5 << 0.4;
    prop[2] << 0.5 << 0.4 << 0.3;
    expected[0] << 0.5;
    expected[1] << 0.4 << 0.3;
    expected[2] << 0.345 << 0.260 << 0.185;
    result[0] = Forage::forage(prop[0]);
    result[1] = Forage::forage(prop[1]);
    result[2] = Forage::forage(prop[2]);
    QVERIFY(compareVectors(result[0], expected[0]));
    QVERIFY(compareVectors(result[1], expected[1]));
    QVERIFY(compareVectors(result[2], expected[2]));
}

bool TestForage::compareVectors(QVector<double> v1, QVector<double> v2) {
    int n = v1.size();
    if (n != v2.size()) {
        std::cout << "Unequal vector sizes: "
                  << v1.size() << " vs. " << v2.size() << "\n";
        return false;
    }
    for (int i = 0; i < n; ++i) {
        if (TestNum::ne(v1[i], v2[i])) {
            std::cout << "Unequal values at index " << i << ": "
                      << v1[i] << " vs. " << v2[i] << "\n";
            return false;
        }
    }
    return true;
}


