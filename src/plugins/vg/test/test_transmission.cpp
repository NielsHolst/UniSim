#include <usbase/test_num.h>
#include "create.h"
#include "test_transmission.h"

using namespace UniSim;

void TestTransmission::init() {
    sim = vg::create::simulation("test_transmission.xml");
}

void TestTransmission::cleanup() {
    delete sim;
}

void TestTransmission::setup(double screenState) {
    screenControl = sim->seekOne<Model*>("screens/wholeRoof/control");
    screenControl->pushValue<double>("signal", screenState);

    sim->execute();

    geometry = sim->seekOne<Model*>("geometry");
    anyCover = sim->seekOne<Model*>("construction/side1");
    screen = sim->seekOne<Model*>("screens/wholeRoof");
    trans = sim->seekOne<Model*>("transmission");

    totalArea = geometry->pullValue<double>("coverArea"),
    roofArea =  geometry->pullValue<double>("roofArea"),
    coverU = anyCover->pullValue<double>("U"),
    coverHaze = anyCover->pullValue<double>("Haze"),
    coverDiffuseTransmission = anyCover->pullValue<double>("diffuseTransmission"),
    screenReduction = screen->pullValue<double>("energyLossReduction"),
    screenLightTransmission = screen->pullValue<double>("LightTransmission"),
    f = roofArea/totalArea;
}

// Screens are drawn with a delay, hence the comparisons cannot be exact.

void TestTransmission::testCommonKFullScreen() {
    setup(1);
    double computedU = trans->pullValue<double>("U"),
           expectedU = f*coverU*(1-screenReduction) + (1-f)*coverU;
    QString s = "Computed U ('%1) != expected U ('%2')";
    QVERIFY2(TestNum::eq(computedU,expectedU, 1e-3), s.arg(computedU).arg(expectedU).toLatin1());
}

void TestTransmission::testCommonKPartScreen() {
    setup(0.2);
    double computedU = trans->pullValue<double>("U"),
           expectedU = f*coverU*(1-screenReduction*0.2) + (1-f)*coverU;
    QString s = "Computed U ('%1) != expected U ('%2')";
    QVERIFY2(TestNum::eq(computedU,expectedU, 1e-3), s.arg(computedU).arg(expectedU).toLatin1());
}

void TestTransmission::testCommonHazeFullScreen() {
    setup(1);
    double computedHaze = trans->pullValue<double>("Haze"),
           expectedHaze = f*(1-(1-coverHaze)*0) + (1-f)*coverHaze;
    QString s = "Computed haze ('%1) != expected haze ('%2')";
    QVERIFY2(TestNum::eq(computedHaze,expectedHaze, 1e-3), s.arg(computedHaze).arg(expectedHaze).toLatin1());
}

void TestTransmission::testCommonHazePartScreen() {
    setup(0.2);
    double computedHaze = trans->pullValue<double>("Haze"),
           expectedHaze = f*(1-(1-coverHaze)*0.8) + (1-f)*coverHaze;
    QString s = "Computed haze ('%1) != expected haze ('%2')";
    QVERIFY2(TestNum::eq(computedHaze,expectedHaze, 1e-3), s.arg(computedHaze).arg(expectedHaze).toLatin1());
}

void TestTransmission::testCommonDiffuseTransmissionFullScreen() {
    setup(1);
    double computedTrans = trans->pullValue<double>("diffuse"),
           expectedTrans = f*coverDiffuseTransmission*screenLightTransmission
                           + (1-f)*coverDiffuseTransmission;
    QString s = "Computed diffuseTransmission ('%1) != expected diffuseTransmission ('%2')";
    QVERIFY2(TestNum::eq(computedTrans,expectedTrans, 1e-3), s.arg(computedTrans).arg(expectedTrans).toLatin1());
}

void TestTransmission::testCommonDiffuseTransmissionPartScreen() {
    setup(0.2);
    double computedTrans = trans->pullValue<double>("diffuse"),
           expectedTrans = f*(coverDiffuseTransmission*screenLightTransmission*0.2
                              + coverDiffuseTransmission*0.8)
                           + (1-f)*coverDiffuseTransmission;
    QString s = "Computed diffuseTransmission ('%1) != expected diffuseTransmission ('%2')";
    QVERIFY2(TestNum::eq(computedTrans,expectedTrans, 1e-3), s.arg(computedTrans).arg(expectedTrans).toLatin1());
}
