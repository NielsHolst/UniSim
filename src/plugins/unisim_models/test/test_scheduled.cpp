#include <iostream>
#include <usbase/utilities.h>
#include <usbase/test_num.h>
#include <usengine/simulation.h>
#include <usengine/simulation_maker.h>
#include <usengine/test_dataset.h>
#include "test_scheduled.h"

using namespace UniSim;

void TestScheduled::cleanup() {
    delete sim;
    sim = 0;
}

void TestScheduled::testSchedule() {
    createSimulation("test_scheduled.xml");
    try {
        sim->execute();
    }
    catch (Exception &ex) {
        QString msg = "Could not execute: " + ex.message();
        QFAIL(qPrintable(msg));
    }

    TestDataset data(30,3);
    try {
        data.read("test_scheduled.prn");;
    }
    catch (Exception &ex) {
        QString msg = "Error reading simulation output: " + ex.message();
        QFAIL(qPrintable(msg));
    }

    const int V = 1;
    const int I = 2;
    QCOMPARE(data.value(0,V), 10.);
    QCOMPARE(data.value(0,I), 0.);

    QCOMPARE(data.value(1,V), 10.);
    QCOMPARE(data.value(1,I), 0.);

    QCOMPARE(data.value(4,V), 10.);
    QCOMPARE(data.value(4,I), 0.);

    QVERIFY(TestNum::eq(data.value(5,V), 10. + 20./15.));
    QVERIFY(TestNum::eq(data.value(5,I), 20./15.));

    QCOMPARE(data.value(19,V), 30.);
    QVERIFY(TestNum::eq(data.value(19,I), 20./15.));

    QVERIFY(TestNum::eq(data.value(23,V), 20.+2. ));
    QVERIFY(TestNum::eq(data.value(23,I), -2.));

    QCOMPARE(data.value(24,V), 20.);
    QVERIFY(TestNum::eq(data.value(24,I), -2.));

    QCOMPARE(data.value(25,V), 20.);
    QCOMPARE(data.value(25,I), 0.);

    QCOMPARE(data.value(29,V), 20.);
    QCOMPARE(data.value(29,I), 0.);
}

//
// Supporting methods
//

void TestScheduled::createSimulation(QString fileName) {
    SimulationMaker maker;
    sim = maker.parse(filePath(fileName));
}

QString TestScheduled::filePath(QString fileName) {
    return QFileInfo(inputFolder(), fileName).absoluteFilePath();
}

QDir TestScheduled::inputFolder() {
    QDir testDir = QDir("../..");
    testDir.cd("src/plugins/unisim_models/test/input");
    Q_ASSERT(testDir.exists());
    return testDir;
}

