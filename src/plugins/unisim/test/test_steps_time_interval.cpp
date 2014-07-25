#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/model.h>
#include <usengine/simulation.h>
#include <usengine/simulation_maker.h>
#include "test_steps_time_interval.h"

using namespace UniSim;

void TestStepsTimeInterval::cleanup() {
    delete sim;
    sim = 0;
}

void TestStepsTimeInterval::testSum() {
    createSimulation("test_steps_time_interval.xml");
    try {
        sim->execute();
    }
    catch (Exception &ex) {
        QString msg = "Could not execute: " + ex.message();
        QFAIL(qPrintable(msg));
    }
    int maxSteps = sim->seekOne<Model*>("steps")->pullValue<int>("maxSteps");
    QCOMPARE(maxSteps, 144 + 16*144/24);
}


//
// Supporting methods
//

void TestStepsTimeInterval::createSimulation(QString fileName) {
    QDir dir = FileLocations::location(FileLocationInfo::Plugins);
    QString filePath = dir.absolutePath() + "/unisim/test/input/" + fileName;
    SimulationMaker maker;
    sim = maker.parse(filePath);
}

