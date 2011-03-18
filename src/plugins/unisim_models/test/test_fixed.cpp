#include <iostream>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/model.h>
#include <usbase/parameter.h>
#include <usbase/utilities.h>
#include <usengine/simulation.h>
#include <usengine/simulation_maker.h>
#include "test_fixed.h"

using namespace UniSim;


void TestFixed::cleanup() {
    delete sim;
    sim = 0;
}

void TestFixed::testPullVariables() {
    createSimulation("test_fixed.xml");
    try {
        sim->execute();
    }
    catch (Exception &ex) {
        QString msg = "Could not execute: " + ex.message();
        QFAIL(qPrintable(msg));
    }

    try {
        Model *empty = seekOneDescendant<Model*>("empty", sim);
        QVERIFY(empty);

        Model *model = seekOneDescendant<Model*>("fixed", sim);
        QVERIFY(model);

        double Tavg = model->pullVariable<double>("Tavg");
        double I = model->pullVariable<double>("I");
        QCOMPARE(Tavg, 22.5);
        QCOMPARE(I, 32.);

        model->pushVariable<double>("m", 45.);
        model->pushVariable<double>("f", 67.);

        double A = model->parameter<double>("A");
        double p = model->parameter<double>("p");
        QCOMPARE(A, 6.023e23);
        QCOMPARE(p, 3.1214);
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception: " + ex.message();
        QFAIL(qPrintable(msg));
    }
}


//
// Supporting methods
//

void TestFixed::createSimulation(QString fileName) {
    QDir dir = FileLocations::location(FileLocationInfo::Plugins);
    QString filePath = dir.absolutePath() + "/unisim_models/test/" + fileName;
    SimulationMaker maker;
    sim = maker.parse(filePath);
}

