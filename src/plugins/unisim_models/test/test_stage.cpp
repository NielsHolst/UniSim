#include <usbase/model.h>
#include <usbase/test_num.h>
#include <usengine/model_maker.h>
#include "../stage.h"
#include "test_stage.h"


using namespace UniSim;

namespace {
    const double L = 50;
    const int k = 30;
}

void TestStage::initTestCase()
{
    model = ModelMaker::create("UniSim::Stage", "adults");
    ModelMaker::create("UniSim::Days", "time", model);
    model->seekOneChild<Parameter<int>*>("k") -> setValue(k);
    model->seekOneChild<Parameter<double>*>("duration") -> setValue(L);
    model->amend();
    model->initialize();
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

    double test = classes[0] + classes[k-1];
    QVERIFY(TestNum::eqZero(test));
}
