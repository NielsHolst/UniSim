#include <iostream>
#include <QDir>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/model.h>
#include <usbase/test_num.h>
#include <usbase/utilities.h>
#include <usengine/simulation.h>
#include <usengine/simulation_maker.h>
#include "test_infection.h"

using namespace std;
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

//    cout << inf->pullValue<double>("Fly-io-attacks") << endl
//        << inf->pullValue<double>("Fly-io-attacked") << endl
//        << inf->pullValue<double>("Fly-attacks") << endl
//        << inf->pullValue<double>("Fly-attacked") << endl
//        << inf->pullValue<double>("Wasp-io_fly-attacks") << endl
//        << inf->pullValue<double>("paphia-attacks") << endl
//        << inf->pullValue<double>("paphia-attacked") << endl
//        << inf->pullValue<double>("Wasp-io_fly-attacks") << endl;
//    cout  << inf->pullValue<double>("fly-io_fly-attacks") << endl;
//    cout  << inf->pullValue<double>("fly-io_fly-attacked") << endl;
//    cout  << inf->pullValue<double>("wasp-io_fly-attacks") << endl;
//    cout  << inf->pullValue<double>("wasp-io_fly-attacked") << endl;
//    cout  << inf->pullValue<double>("io_fly-attacks") << endl;
//    cout  << inf->pullValue<double>("io_fly-attacked") << endl;

    QVERIFY(TestNum::eq(inf->pullValue<double>("Fly-io-attacks"), 3.4995, 1e-4));
    QVERIFY(TestNum::eq(inf->pullValue<double>("Fly-io-attacked"), 3.4389, 1e-4));
    QVERIFY(TestNum::eq(inf->pullValue<double>("Fly-attacks"), 5.9408, 1e-4));
    QVERIFY(TestNum::eq(inf->pullValue<double>("Fly-attacked"), 5.863, 1e-4));
    QVERIFY(TestNum::eq(inf->pullValue<double>("wasp-io_fly-attacks"), 0.0019693, 1e-4));
    QVERIFY(TestNum::eq(inf->pullValue<double>("io_fly-attacks"), 0.0019693, 1e-4));
    QVERIFY(TestNum::eq(inf->pullValue<double>("io_fly-attacked"), 0.0019690, 1e-4));
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
    try {
        sim = maker.parse(filePath);
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.message()));
    }
}

