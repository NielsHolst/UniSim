#include <QDir>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usengine/simulation.h>
#include <usengine/simulation_maker.h>
#include "test_records.h"

using namespace UniSim;

void TestRecords::createSimulation(QString fileName) {
    QDir dir = FileLocations::location(FileLocationInfo::Plugins);
    QString filePath = dir.absolutePath() + "/unisim_models/test/" + fileName;
    SimulationMaker maker;
    sim = maker.parse(filePath);
}

void TestRecords::cleanup() {
    delete sim;
    sim = 0;
}


void TestRecords::testColumnLabels() {
    createSimulation("test_records.xml");
    try {
        sim->execute();
    }
    catch (Exception &ex) {
        QString msg = "Could not execute: " + ex.message();
        QFAIL(qPrintable(msg));
    }
}

