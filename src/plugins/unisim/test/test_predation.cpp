#include <iostream>
#include <QDir>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/parameter.h>
#include <usbase/test_num.h>
#include <usbase/utilities.h>
#include <usengine/simulation.h>
#include <usengine/simulation_maker.h>
#include "test_predation.h"

using namespace UniSim;

void TestPredation::cleanup() {
//    delete sim;
//    sim = 0;
}

void TestPredation::testRecipe() {
    //createSimulation("test_predation.xml");
}

//
// Supporting methods
//

void TestPredation::createSimulation(QString fileName) {
    QDir dir = FileLocations::location(FileLocationInfo::Plugins);
    QString filePath = dir.absolutePath() + "/unisim/test/input/" + fileName;
    SimulationMaker maker;
    sim = maker.parse(filePath);
}

