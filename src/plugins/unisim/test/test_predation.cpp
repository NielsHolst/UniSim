#include <iostream>
#include <QDir>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/model.h>
#include <usbase/test_num.h>
#include <usengine/simulation.h>
#include <usengine/simulation_maker.h>
#include "test_predation.h"

using namespace UniSim;

void TestPredation::cleanup() {
    delete sim;
    sim = 0;
}

void TestPredation::testRecipe() {
    createSimulation("test_predation_2_predators.xml");
}

void TestPredation::testOnePredator() {
    run("test_predation_1_predator.xml");
    Model *pred = sim->seekOne<Model*>("predation");
    QVERIFY(TestNum::eq(pred->pullValue<double>("Leopard-Gazella"), 4.2604, 1e-4));
    QVERIFY(TestNum::eq(pred->pullValue<double>("Gnu-Loss"), 3.4791, 1e-4));
    QVERIFY(TestNum::eqZero(pred->pullValue<double>("Leopard-Loss")));
    QVERIFY(TestNum::eq(pred->pullValue<double>("Leopard-Supply"), 11.999, 1e-4));
}

void TestPredation::testTwoPredators() {
    run("test_predation_2_predators.xml");
    Model *pred = sim->seekOne<Model*>("predation");
    std::cout
        << pred->pullValue<double>("Leopard-Gazella")   << "\n"
        << pred->pullValue<double>("Gnu-Loss")<< "\n"
        << pred->pullValue<double>("Leopard-Loss")<< "\n"
        << pred->pullValue<double>("Leopard-Supply")<< "\n"
        << pred->pullValue<double>("Lion-Gazella")<< "\n"
        << pred->pullValue<double>("Lion-Leopard")<< "\n";
    QVERIFY(TestNum::eq(pred->pullValue<double>("Leopard-Gazella"), 3.4104, 1e-4));
    QVERIFY(TestNum::eq(pred->pullValue<double>("Gnu-Loss"),  1.6597, 1e-4));
    QVERIFY(TestNum::eq(pred->pullValue<double>("Leopard-Loss"),  0.0019693, 1e-4));
    QVERIFY(TestNum::eq(pred->pullValue<double>("Leopard-Supply"), 5.8207, 1e-4));

    QVERIFY(TestNum::eq(pred->pullValue<double>("Lion-Gazella"), 4.9991, 1e-4));
    QVERIFY(TestNum::eq(pred->pullValue<double>("Lion-Leopard"), 0.0019693, 1e-4));
}


//
// Supporting methods
//

void TestPredation::run(QString fileName) {
    createSimulation(fileName);
    try {
        sim->execute();
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.message()));
    }
}

void TestPredation::createSimulation(QString fileName) {
    QDir dir = FileLocations::location(FileLocationInfo::Plugins);
    QString filePath = dir.absolutePath() + "/unisim/test/input/" + fileName;
    SimulationMaker maker;
    sim = maker.parse(filePath);
}

