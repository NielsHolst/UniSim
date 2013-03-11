#include <iostream>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/model.h>
#include <usbase/parameter.h>
#include <usbase/utilities.h>
#include <usengine/simulation.h>
#include <usengine/simulation_maker.h>
#include "test_sum.h"

using namespace UniSim;


void TestSum::cleanup() {
    delete sim;
    sim = 0;
}

void TestSum::testSum() {
    createSimulation("test_sum_3.xml");
    try {
        sim->execute();
    }
    catch (Exception &ex) {
        QString msg = "Could not execute: " + ex.message();
        QFAIL(qPrintable(msg));
    }
    double sum = sim->seekOne<Model*>("sum")->pullValue<double>("value");
    QCOMPARE(sum,10.+100.+1000.);
}


//
// Supporting methods
//

void TestSum::createSimulation(QString fileName) {
    QDir dir = FileLocations::location(FileLocationInfo::Plugins);
    QString filePath = dir.absolutePath() + "/unisim/test/input/" + fileName;
    SimulationMaker maker;
    sim = maker.parse(filePath);
}

