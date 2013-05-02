#include <iostream>
#include <QDir>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/model.h>
#include <usbase/test_num.h>
#include <usengine/simulation.h>
#include <usengine/simulation_maker.h>
#include "test_infection.h"

using namespace UniSim;

void TestInfection::cleanup() {
    delete sim;
    sim = 0;
}

void TestInfection::testRecipe() {
    createSimulation("test_infection_2_parasitoids.xml");
}

void TestInfection::testTwoParasitoids() {
    run("test_infection_2_parasitoids.xml");
    Model *inf = sim->seekOne<Model*>("infection");
    QVERIFY(TestNum::eq(inf->pullValue<double>("Leopard-Gazella-NumAttacks"), 4.1267, 1e-4));
    QVERIFY(TestNum::eq(inf->pullValue<double>("Lion-Leopard-NumAttacks"), 0.036056, 1e-4));

    QVERIFY(TestNum::eq(inf->pullValue<double>("Leopard-Gazella-AttacksPerHost"), 0.041267, 1e-4));
    QVERIFY(TestNum::eq(inf->pullValue<double>("Lion-Leopard-AttacksPerHost"), 0.0072112, 1e-4));

    QVERIFY(TestNum::eq(inf->pullValue<double>("Leopard-Gazella-NumHostsAttacked"), 4.0427, 1e-4));
    QVERIFY(TestNum::eq(inf->pullValue<double>("Lion-Leopard-NumHostsAttacked"), 0.035926, 1e-4));

    QVERIFY(TestNum::eq(inf->pullValue<double>("Leopard-Gazella-PropHostsAttacked"), 0.040427, 1e-4));
    QVERIFY(TestNum::eq(inf->pullValue<double>("Lion-Leopard-PropHostsAttacked"), 0.0071853, 1e-4));
}


//
// Supporting methods
//

void TestInfection::run(QString fileName) {
    createSimulation(fileName);
    try {
        sim->execute();
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.message()));
    }
}

void TestInfection::createSimulation(QString fileName) {
    QDir dir = FileLocations::location(FileLocationInfo::Plugins);
    QString filePath = dir.absolutePath() + "/unisim/test/input/" + fileName;
    SimulationMaker maker;
    sim = maker.parse(filePath);
}

