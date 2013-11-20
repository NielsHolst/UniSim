#include <usbase/exception.h>
#include <usbase/data_grid.h>
#include <usengine/simulation.h>
#include <usengine/simulation_maker.h>
#include "test_data_grid.h"

using std::cout;
using namespace UniSim;

void TestDataGrid::testNoKeys() {
    DataGrid data( filePath("data_grid_0_keys.txt") );
    QCOMPARE(data.numKeys(), 0);
    QCOMPARE(data.cell(0,0), QString("barley"));
    QCOMPARE(data.cell(3,4), QString("no"));
}

void TestDataGrid::test1D() {
    DataGrid data( filePath("data_grid_1_key.txt") );
    QCOMPARE(data.numKeys(), 1);
    QCOMPARE(data.cell(0,0), QString("barley"));
    QCOMPARE(data.cell(3,4), QString("no"));
    QStringList keys;
    keys << "barley";
    QString test = data.cell(keys, QString("Nitrogen"));
    QCOMPARE(test, QString("90.1"));
    QCOMPARE(data.cell(keys, QString("Nitrogen")), QString("90.1"));
}

void TestDataGrid::test2D() {
    DataGrid data( filePath("data_grid_2_keys.txt") );
    QCOMPARE(data.numKeys(), 2);
    QCOMPARE(data.cell(0,0), QString("barley"));
    QCOMPARE(data.cell(3,4), QString("no"));
    QStringList keys;
    keys << "wheat" << "clay";
    QCOMPARE(data.cell(keys, QString("Nitrogen")), QString("80.2"));
}

void TestDataGrid::testOneLineFile() {
    DataGrid data( filePath("data_grid_one_line.txt") );
    QStringList keys;
    keys << "barley" << "sandy";
    QCOMPARE(data.cell(keys, QString("Treatment")), QString("1"));
}
void TestDataGrid::testMissingFile() {
    bool excepted = false;
    try {
        new DataGrid("missing_file.txt");
    }
    catch (Exception &) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestDataGrid::testEmptyFile() {
    bool excepted = false;
    try {
        new DataGrid("data_grid_empty.txt");
    }
    catch (Exception &) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestDataGrid::testIllFormedFile() {
    bool excepted = false;
    try {
        new DataGrid("data_grid_ill_formed.txt");
    }
    catch (Exception &) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestDataGrid::testOnlyColumnHeadings() {
    DataGrid data( filePath("data_grid_only_column_headings.txt") );
    QCOMPARE(data.rowNames().size(), 0);
}

void TestDataGrid::testAutoColumnHeadingMissing() {
    DataGrid data( filePath("data_grid_auto_column_heading_missing.txt") );
    QCOMPARE(data.numKeys(), 1);
    QStringList keys;
    keys << "barley";
    QCOMPARE(data.cell(keys, QString("Nitrogen")), QString("90.1"));
}

void TestDataGrid::testAutoColumnHeadingEmpty() {
    DataGrid data( filePath("data_grid_auto_column_heading_empty.txt") );
    QCOMPARE(data.numKeys(), 1);
    QStringList keys;
    keys << "barley";
    QCOMPARE(data.cell(keys, QString("Nitrogen")), QString("90.1"));
}

void TestDataGrid::testWrongKeys() {
    bool excepted = false;
    try {
        new DataGrid("data_grid_wrong_keys.txt");
    }
    catch (Exception &) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestDataGrid::testRowIndices() {
    DataGrid data( filePath("data_grid_2_keys.txt") );
    DataGrid::KeySubset keys;
    for (int i = 0; i < 3; ++i) {
        keys[1] = "clay";
        QList<int> rows = data.rowIndices(keys);
        QCOMPARE(rows.size(), 2);
        QCOMPARE(rows.value(0), 1);
        QCOMPARE(rows.value(1), 3);
    }

    keys.clear();
    keys[1] = "none";
    QList<int> rows = data.rowIndices(keys);
    QCOMPARE(rows.size(), 0);
}

void TestDataGrid::testColumnString() {
    DataGrid data( filePath("data_grid_0_keys.txt") );
    QList<QString> soilType = QList<QString>::fromVector(data.column<QString>(1));
    QList<QString> expected;
    expected << "sandy" << "clay" << "sandy" << "clay";
    QCOMPARE(soilType, expected);
}

void TestDataGrid::testColumnInt() {
    DataGrid data( filePath("data_grid_0_keys.txt") );
    QList<int> water= QList<int>::fromVector(data.column<int>(3));
    QList<int> expected;
    expected << 70 << 75 << 50 << 85;
    QCOMPARE(water, expected);
}

//
// Helpers
//

QString TestDataGrid::filePath(QString fileName) {
    return QFileInfo(inputFolder(), fileName).absoluteFilePath();
}

QDir TestDataGrid::inputFolder() {
    QDir testDir = QDir("../..");
    testDir.cd("src/library/usbase/test/input");
    Q_ASSERT(testDir.exists());
    return testDir;
}





