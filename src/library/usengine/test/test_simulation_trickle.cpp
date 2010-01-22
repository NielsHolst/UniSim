#include <usbase/controller.h>
#include <usbase/identifier.h>
#include <usbase/utilities.h>
#include <usengine/simulation.h>
#include <standard_controllers/simple_controller.h>
#include "test_simulation_trickle.h"
#include "trickle_box.h"
#include "trickle_sequence.h"

using namespace UniSim;

void TestSimulationTrickle::initTestCase() {
    simulation = new Simulation("trickles", "1.0");
    setSimulationObject(simulation);
    controller = new SimpleController("controller", simulation);

    TrickleSequence *seq;
    seq = new TrickleSequence("sequence", simulation);
    boxes[0] = new TrickleBox("box0", seq);
    boxes[1] = new TrickleBox("box1", seq);
    boxes[2] = new TrickleBox("box2", seq);

    simulation->initialize(Identifiers() << "sequence");
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
    controller->changeParameter("numSteps", steps);
    simulation->execute();
    textBox(0, check0);
    textBox(1, check1);
    textBox(2, check2);
}

void TestSimulationTrickle::textBox(int boxNumber, int contents) {
    QList<Model*> models = UniSim::find<Model*>("box"+QString::number(boxNumber));
    QCOMPARE(models.size(), 1);
    QVERIFY(models[0]->statePtr("contents") != 0);
    QCOMPARE(models[0]->state("contents"), double(contents));

    QVERIFY(boxes[boxNumber]->statePtr("contents") != 0);
    QCOMPARE(boxes[boxNumber]->state("contents"), double(contents));
}

