#include <usbase/model.h>
#include <usbase/test_num.h>
#include <usengine/mega_factory.h>
#include "test_adjustable_setpoint.h"

using namespace UniSim;

void TestAdjustableSetpoint::testIncrement() {
    Model *sp = MegaFactory::create<Model>("vg::AdjustableSetpoint", "sp", 0);
    sp->pushValue<double>("baseSetpoint", 25.);
    sp->pushValue<double>("adjustment", 10.);
    sp->run();
    QVERIFY(TestNum::eq(sp->pullValue<double>("setpoint"), 35.));
    delete sp;
}

void TestAdjustableSetpoint::testDecrement() {
    Model *sp = MegaFactory::create<Model>("vg::AdjustableSetpoint", "sp", 0);
    sp->pushValue<double>("baseSetpoint", 25.);
    sp->pushValue<double>("adjustment", 10.);
    sp->pushValue<QString>("direction", "decrement");
    sp->run();
    QVERIFY(TestNum::eq(sp->pullValue<double>("setpoint"), 15.));
    delete sp;
}
