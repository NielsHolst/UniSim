#include <usbase/data_grid.h>
#include <usbase/model.h>
#include <usengine/simulation.h>
#include <usengine/simulation_maker.h>
#include "test_table.h"

using namespace UniSim;

void TestTable::test3Levels() {
    QString filePath = TestTable::filePath("models_from_file_nested_twice.xml");
    SimulationMaker maker;
    Simulation * sim = 0;
    try {
        sim = maker.parse(filePath);
        sim->execute();
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception. " + ex.message();
        QFAIL(qPrintable(msg));
    }
    Models all = sim->seekDescendants<Model*>("*");
    QCOMPARE(all.size(), 1+1+1+4+7+4+16);

    DataGrid* table = openOutput("test_3levels_output.txt");
    QCOMPARE(table->rowNames().size(), 10);
    QCOMPARE(table->columnNames().size(), 3);
    QCOMPARE(table->cell(0,2).toInt(), 101);
    QCOMPARE(table->cell(9,2).toInt(), 110);
    QStringList expectedColumnNames;
    expectedColumnNames << "time" << "x" << "labour";
    QCOMPARE(table->columnNames(), expectedColumnNames);
    delete table;

    table = openOutput("test_3levels_jokers_output.txt");
    QCOMPARE(table->rowNames().size(), 10);
    QCOMPARE(table->columnNames().size(), 18);
    QStringList colNames = table->columnNames();
    QCOMPARE(colNames[2], QString("A/Oats/Sowing/labour"));
    QCOMPARE(colNames[17], QString("D/Oats/Harvest/labour"));
    delete table;
}

QString TestTable::filePath(QString fileName) const {
    QDir uniSimDir = QDir("../..");
    return uniSimDir.absolutePath() +
            "/src/library/usengine/test/input/simulation_maker/" +
            fileName;
}

DataGrid* TestTable::openOutput(QString fileName) const {
    QString path = FileLocations::location(FileLocationInfo::Output).absolutePath();
    QString filePath = path + "/" + fileName;
    return new DataGrid(filePath);
}
