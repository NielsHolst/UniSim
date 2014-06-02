#include <iostream>
#include <usbase/data_grid.h>
#include <usbase/exception.h>
#include <usbase/test_num.h>
#include <usbase/interpolate.h>
#include "test_interpolate.h"

using namespace UniSim;

void TestInterpolate::testBracketEmpty() {
    QVector<int> x;
    bool excepted{false};
    try {
        bracket(x, 7);
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestInterpolate::testBracketOne() {
    QVector<int> x{10};
    auto b = bracket(x, 7);
    QCOMPARE(b.first, 0);
    QCOMPARE(b.second, 0);
}

void TestInterpolate::testBracketTwo() {
    QVector<int> x{10,20};
    auto b = bracket(x, 17);
    QCOMPARE(b.first, 0);
    QCOMPARE(b.second, 1);
}

void TestInterpolate::testBracketEqual() {
    QVector<int> x{10,20,30,40};
    auto b = bracket(x, 30);
    QCOMPARE(b.first, 1);
    QCOMPARE(b.second, 2);
}

void TestInterpolate::testBracketBetween() {
    QVector<int> x{10,20,30,40};
    auto b = bracket(x, 35);
    QCOMPARE(b.first, 2);
    QCOMPARE(b.second, 3);
}

void TestInterpolate::testBracketBelow() {
    QVector<int> x{10,20,30,40};
    auto b = bracket(x, 5);
    QCOMPARE(b.first, 0);
    QCOMPARE(b.second, 0);
}

void TestInterpolate::testBracketAbove() {
    QVector<int> x{10,20,30,40};
    auto b = bracket(x, 45);
    QCOMPARE(b.first, 3);
    QCOMPARE(b.second, 3);
}

void TestInterpolate::testInterpolateTwoPointsIncreasing(){
    QPair<int,int> p1{10, 100}, p2{20, 500};
    double y = interpolate(p1, p2, 12);
    QVERIFY(TestNum::eq(y, 180.));
}

void TestInterpolate::testInterpolateTwoPointsDecreasing() {
    QPair<int,int> p1{10, 100}, p2{20, 500};
    double y = interpolate(p2, p1, 12);
    QVERIFY(TestNum::eq(y, 180.));

}

void TestInterpolate::testInterpolateTwoPointsEqual() {
    QPair<int,int> p1{10, 100};
    bool excepted{false};
    try {
        interpolate(p1, p1, 12);
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestInterpolate::testInterpolateTwoPointsBelow() {
    QPair<int,int> p1{10, 100}, p2{20, 500};
    double y = interpolate(p2, p1, 8);
    QVERIFY(TestNum::eq(y, 20.));
}

void TestInterpolate::testInterpolateTwoPointsAbove() {
    QPair<int,int> p1{10, 100}, p2{20, 500};
    double y = interpolate(p2, p1, 22);
    QVERIFY(TestNum::eq(y, 580.));
}

void TestInterpolate::testInterpolateManyEmpty() {
    QMap<int,int> p;
    bool excepted{false};
    try {
        interpolate(p, 12);
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestInterpolate::testInterpolateManyOne() {
    QMap<int,int> p{{10,100}};
    double y = interpolate(p, 12);
    QVERIFY(TestNum::eq(y, 100.));
}

void TestInterpolate::testInterpolateManyTwo() {
    QMap<int,int> p{{10,100},{20,500}};
    double y = interpolate(p, 12);
    QVERIFY(TestNum::eq(y, 180.));
}

void TestInterpolate::testInterpolateManyEqual() {
    QMap<int,int> p{{10,100},{20,500}, {30,1000}, {40,2000}};
    double y = interpolate(p, 30);
    QVERIFY(TestNum::eq(y, 1000.));
}

void TestInterpolate::testInterpolateManyBetween() {
    QMap<int,int> p{{10,100},{20,500}, {30,1000}, {40,2000}};
    double y = interpolate(p, 32);
    QVERIFY(TestNum::eq(y, 1200.));
}

void TestInterpolate::testInterpolateManyBelow() {
    QMap<int,int> p{{10,100},{20,500}, {30,1000}, {40,2000}};
    double y = interpolate(p, 5);
    QVERIFY(TestNum::eq(y, 100.));
}

void TestInterpolate::testInterpolateManyAbove() {
    QMap<int,int> p{{10,100},{20,500}, {30,1000}, {40,2000}};
    double y = interpolate(p, 45);
    QVERIFY(TestNum::eq(y, 2000.));
}

const double a = 8 + 26./4, b = 13 + 42./4, c = a + (b-a)*0.4;

void TestInterpolate::testInterpolatePlane2x2() {
    DataGrid m( filePath("interpolate_plane_2x2.txt") );
    double y;
    // Mid
    y = interpolate(m, 325, 24);
    QVERIFY(TestNum::eq(y, c));
    // Left
    y = interpolate(m, 325, 20);
    QVERIFY(TestNum::eq(y, a));
    // Right
    y = interpolate(m, 325, 30);
    QVERIFY(TestNum::eq(y, b));
    // Top
    y = interpolate(m, 300, 24);
    QVERIFY(TestNum::eq(y, 8 + 5*0.4));
    // Bottom
    y = interpolate(m, 400, 24);
    QVERIFY(TestNum::eq(y, 34 + 21*0.4));
    // Upper left
    y = interpolate(m, 300, 20);
    QVERIFY(TestNum::eq(y, 8));
    // Upper right
    y = interpolate(m, 300, 30);
    QVERIFY(TestNum::eq(y, 13));
    // Lower left
    y = interpolate(m, 400, 20);
    QVERIFY(TestNum::eq(y, 34));
    // Lower right
    y = interpolate(m, 400, 30);
    QVERIFY(TestNum::eq(y, 55));
}

void TestInterpolate::testInterpolatePlane5x4() {
    DataGrid m( filePath("interpolate_plane_5x4.txt") );
    double y = interpolate(m, 325, 24);
    QVERIFY(TestNum::eq(y, c));
}


//
// Helpers
//

QString TestInterpolate::filePath(QString fileName) {
    return QFileInfo(inputFolder(), fileName).absoluteFilePath();
}

QDir TestInterpolate::inputFolder() {
    QDir testDir = QDir("../..");
    testDir.cd("src/library/usbase/test/input");
    Q_ASSERT(testDir.exists());
    return testDir;
}

