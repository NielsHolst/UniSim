#include <usbase/model.h>
#include <usengine/model_maker.h>
#include "../stage.h"
#include "test_stage.h"


using namespace UniSim;

namespace {
    const double L = 50;
    const int K = 30;
}

void TestStage::initTestCase()
{
    model = ModelMaker::create("UniSim::Stage", "adults");
    ModelMaker::create("UniSim::Days", "time", model);
    model->initialize();
    model->seekOneChild<Parameter<int>*>("k") -> setValue(K);
    model->seekOneChild<Parameter<double>*>("duration") -> setValue(L);
}

void TestStage::cleanupTestCase()
{
    delete model;
}

void TestStage::testAgeClasses()
{
    Stage *stage = dynamic_cast<Stage*>(model);
    double const *classes;

    stage->reset();
    classes = stage->pullVariablePtr<double>("ageClasses");
    QVERIFY(classes);

    model->seekOneChild<Parameter<int>*>("k") -> setValue(300);
    stage->reset();
    classes = stage->pullVariablePtr<double>("ageClasses");
    QVERIFY(classes);
    double test = classes[0] + classes[299];
    QCOMPARE(test, 0.);
}
