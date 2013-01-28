#include <iostream>
#include <usbase/test_num.h>
#include <usengine/mega_factory.h>
#include "../functional_response_g_b.h"
#include "test_functional_response_g_b.h"

using namespace UniSim;


void TestFunctionalResponseGB::testPredation() {
    Model *model = MegaFactory::create<FunctionalResponseGB>("UniSim::FunctionalResponseGB", "funcResp", 0);
    QVERIFY(model);

    model->pushValue("apparancy",0.8);
    model->pushValue("resourceDensity",30.);
    model->pushValue("demand",10.);
    model->pushValue("egestionRatio",0.2);
    model->pushValue("respiration",1.);
    model->update();
    double supply = model->pullValue<double>("supply");
    double sdRatio = model->pullValue<double>("sdRatio");
    double demand = model->pullValue<double>("demand");

    QVERIFY(TestNum::eq(supply, 8.0798, 1e-04));
    QVERIFY(TestNum::eq(sdRatio, supply/demand, 1e-04));

    delete model;
}

void TestFunctionalResponseGB::testInfection() {
    Model *model = MegaFactory::create<FunctionalResponseGB>("UniSim::FunctionalResponseGB", "funcResp", 0);
    QVERIFY(model);

    model->pushValue("apparancy",0.8047);
    model->pushValue("resourceDensity",20.);
    model->pushValue("demand",10.);
    model->update();
    double supply = model->pullValue<double>("supply");

    QVERIFY(TestNum::eq(supply, 8., 1e-4));
    QVERIFY(TestNum::eq(model->pullValue<double>("attacksPerHost"), 0.4, 1e-4));
    QVERIFY(TestNum::eq(model->pullValue<double>("numHostsAttacked"), 6.5936, 1e-4));
    QVERIFY(TestNum::eq(model->pullValue<double>("propHostsAttacked"), 0.3297, 1e-4));

    delete model;
}
