#include <usbase/integrator.h>
#include <usbase/identifier.h>
#include <usbase/parameter.h>
#include <usbase/utilities.h>
#include <usengine/simulation.h>
#include <unisim/steps.h>
#include "test_simulation_trickle.h"
#include "trickle_box.h"
#include "trickle_sequence.h"

using namespace UniSim;

void TestSimulationTrickle::initTestCase() {
    simulation = new Simulation("trickles");
    setSimulationObject(simulation);
    integrator = new Steps("integrator", simulation);
}

void TestSimulationTrickle::cleanupTestCase() {
    delete simulation;
}


void TestSimulationTrickle::testExecute() {
    executeAndTest(0, 0, 0, 0);
    /*executeAndTest(1, 0, 0, 0);
    executeAndTest(5, 0, 0, 0);
    executeAndTest(6, 0, 0, 0);
    executeAndTest(14, 0, 0, 0);
    executeAndTest(15, 0, 0, 0);
    executeAndTest(20, 0, 0, 0);*/
}

void TestSimulationTrickle::executeAndTest(int steps, int check0, int check1, int check2) {
    Parameter<int> *maxSteps = seekOneChild<Parameter<int> *>("maxSteps", integrator);
    maxSteps->setValue(steps);
    simulation->execute();
    testBox(0, check0);
    testBox(1, check1);
    testBox(2, check2);
}

void TestSimulationTrickle::testBox(int boxNumber, int contents) {
    /*
    QString name = "box"+QString::number(boxNumber);
    QList<Model*> models = UniSim::seekDescendants<Model*>(name, 0);
    QCOMPARE(models.size(), 1);

    QVERIFY(models[0]->pullValuePtr<double>("contents") != 0);
    QVERIFY(fabs(models[0]->pullValue<double>("contents") - double(contents)) < 1e-6);

    QVERIFY(boxes[boxNumber]->pullValuePtr<double>("contents") != 0);
    QVERIFY(fabs(boxes[boxNumber]->pullValue<double>("contents") - double(contents)) < 1e-6);
    */
}

