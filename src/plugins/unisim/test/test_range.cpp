#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/test_num.h>
#include <usengine/simulation.h>
#include <usengine/simulation_maker.h>
#include <usengine/test_dataset.h>
#include "test_range.h"

using namespace UniSim;


void TestRange::cleanup() {
    delete sim;
    sim = 0;
}

void TestRange::testLinear() {
    createSimulation("test_range_linear.xml");
    try {
        sim->execute();
    }
    catch (Exception &ex) {
        QString msg = "Could not execute: " + ex.message();
        QFAIL(qPrintable(msg));
    }

    TestDataset data(10,2);
    data.read("test_range_linear.txt");;
    QCOMPARE(data.value(0,1), -34.);
    QCOMPARE(data.value(4,1), -10.);
    QCOMPARE(data.value(9,1), 20.);
}

void TestRange::testLog10() {
    createSimulation("test_range_log10.xml");
    try {
        sim->execute();
    }
    catch (Exception &ex) {
        QString msg = "Could not execute: " + ex.message();
        QFAIL(qPrintable(msg));
    }

    TestDataset data(10,2);
    data.read("test_range_log10.txt");;
    QVERIFY( TestNum::eq( data.value(0,1), pow(10,-3.4) ) );
    QCOMPARE(data.value(4,1), 0.1);
    QCOMPARE(data.value(9,1), 100.);
}


//
// Supporting methods
//

void TestRange::createSimulation(QString fileName) {
    QDir dir = FileLocations::location(FileLocationInfo::Plugins);
    QString filePath = dir.absolutePath() + "/unisim/test/input/" + fileName;
    SimulationMaker maker;
    sim = maker.parse(filePath);
}

