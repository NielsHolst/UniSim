#include <QFileInfo>
#include <usbase/data_grid.h>
#include <usbase/file_locations.h>
#include <usbase/model.h>
#include <usengine/simulation.h>
#include <usengine/simulation_maker.h>
#include "test_crosstab.h"

using namespace UniSim;

void TestCrosstab::initTestCase() {
    QString filename = inputFilePath("models_from_file_nested_twice.xml");
    SimulationMaker maker;
    try {
        sim = maker.parse(filename);
        sim->execute();
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception. " + ex.message();
        QFAIL(qPrintable(msg));
    }
    crops << "Oats" << "SBarley" << "WBarley";
    farms << "A" << "B" << "D";
    farmClass << "test::Farm";
    cropClass << "test::Crop";
}

void TestCrosstab::cleanupTestCase() {
    delete sim;
}

void TestCrosstab::testOutput() {
    QStringList fileNames;
    fileNames << "test_crosstab_output1_0001.txt" << "test_crosstab_output1_0010.txt";

    for (int i = 0; i < fileNames.size(); ++i) {
        checkFileExists(fileNames[i]);
        DataGrid* table = new DataGrid(outputFilePath(fileNames[i]));
        QCOMPARE(table->rowNumber(), 3);
        QCOMPARE(table->columnNumber(), 4);
        QCOMPARE(table->columnNames(), farmClass + crops);
        QCOMPARE(table->cell(0,0), QString("A"));
        QCOMPARE(table->cell(2,0), QString("D"));
        QCOMPARE(table->cell(1,1).toInt(), 20+9*i);
        QCOMPARE(table->cell(1,2), QString("NA"));
        QCOMPARE(table->cell(2,1).toInt(), 55+9*i);
        delete table;
    }
}

void TestCrosstab::testOutputTransposed() {
    QStringList fileNames;
    fileNames << "test_crosstab_output2_0001.txt" << "test_crosstab_output2_0010.txt";

    for (int i = 0; i < fileNames.size(); ++i) {
        checkFileExists(fileNames[i]);
        DataGrid* table = new DataGrid(outputFilePath(fileNames[i]));
        QCOMPARE(table->rowNumber(), 3);
        QCOMPARE(table->columnNumber(), 4);
        QCOMPARE(table->columnNames(), cropClass + farms);
        QCOMPARE(table->cell(0,0), QString("Oats"));
        QCOMPARE(table->cell(2,0), QString("WBarley"));
        QCOMPARE(table->cell(1,1).toInt(), 30+9*i);
        QCOMPARE(table->cell(1,2), QString("NA"));
        QCOMPARE(table->cell(0,3).toInt(), 55+9*i);
        delete table;
    }

}

QString TestCrosstab::inputFilePath(QString fileName) const {
    QDir uniSimDir = QDir("../..");
    return uniSimDir.absolutePath() +
            "/src/plugins/unisim/test/input/" +
            fileName;
}

QString TestCrosstab::outputFilePath(QString fileName) const {
    QString path = FileLocations::location(FileLocationInfo::Output).absolutePath();
    return path + "/" + fileName;
}

void TestCrosstab::checkFileExists(QString fileName) const {
    QFileInfo fileInfo(outputFilePath(fileName));
    QVERIFY(fileInfo.exists());
}

