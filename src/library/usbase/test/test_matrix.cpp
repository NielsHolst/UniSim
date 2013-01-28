#include <usbase/matrix.h>
#include "test_matrix.h"

using namespace UniSim;

void TestMatrix::testEmpty() {
    bool excepted = false;
    Matrix<int> m;
    try {
        m(0,0);
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestMatrix::test1By1() {
    Matrix<int> m(1,1);
    m(0,0) = 12;
    QCOMPARE(m.at(0,0), 12);

    bool excepted = false;
    try {
        m(0,1);
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestMatrix::testNByN() {
    Matrix<int> m(2,3);
    for (int r = 0; r < 2; ++r)
        for (int c = 0; c < 3; ++c)
            m(r,c) = 10*r + c;
    QCOMPARE(m.at(0,0), 0);
    QCOMPARE(m.at(0,1), 1);
    QCOMPARE(m.at(0,2), 2);
    QCOMPARE(m.at(1,0), 10);
    QCOMPARE(m.at(1,1), 11);
    QCOMPARE(m.at(1,2), 12);

    bool excepted = false;
    try {
        m(2,2);
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestMatrix::testResize() {
    Matrix<int> m(2,3);
    m.resize(1,4);
    m(0,0) = 100;
    m(0,1) = 101;
    m(0,2) = 102;
    m(0,3) = 103;
    QCOMPARE(m(0,0), 100);
    QCOMPARE(m(0,1), 101);
    QCOMPARE(m(0,2), 102);
    QCOMPARE(m(0,3), 103);

    bool excepted = false;
    try {
        m(1,1);
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
}
