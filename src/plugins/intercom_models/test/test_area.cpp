#include <iostream>
#include <usbase/clock.h>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/utilities.h>
#include <usengine/simulation.h>
#include <usengine/simulation_maker.h>
#include "../area.h"
#include "../light_components.h"
#include "../photosynthetic_rate.h"
#include "../plant.h"
#include "test_area.h"

using std::cout;
using namespace UniSim;
using namespace intercom;

void TestArea::initTestCase()
{
    QDir dir = FileLocations::location(FileLocationInfo::Models);
    QString path = dir.absolutePath();

    SimulationMaker *maker = new SimulationMaker;
    try {
        QDir dir = FileLocations::location(FileLocationInfo::Plugins);
        QString filePath = dir.absolutePath() + "/intercom_models/test/test_area_canopy_layer.xml";
        simulation = maker->parse(filePath);
        leafArea = seekOneDescendant<Area*>("leaves/area", simulation);
        lightUseEfficiency = seekOneChild<Model*>("lightUseEfficiency", leafArea);
        assimilationMax = seekOneChild<Model*>("amax", leafArea);
        height = seekOneDescendant<Model*>("height", simulation);
        calendar = seekOneDescendant<Model*>("calendar", simulation);
        reset();
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.message()));
    }
    delete maker;
}

void TestArea::reset() {
    Model *calendar, *field;
    calendar = seekOneDescendant<Model*>("calendar", simulation);
    weather = seekOneDescendant<Model*>("weather", simulation);
    field = seekOneDescendant<Model*>("field", simulation);
    calendar->deepReset();
    weather->deepReset();
    field->deepReset();

    calendar->deepUpdate();
    weather->deepUpdate();

    lightUseEfficiency->deepUpdate();
    assimilationMax->deepUpdate();
}


void TestArea::cleanupTestCase()
{
    delete simulation;
}

void TestArea::testAtHeight() {
    double ph = height->pullVariable<double>("height");
    double lai = leafArea->pullVariable<double>("lai");
    QCOMPARE(lai*6./pow(ph,3)*0.2*(ph - 0.2), leafArea->atHeight(0.2));
}

void TestArea::testAboveHeight() {
    double ph = height->pullVariable<double>("height");
    double lai = leafArea->pullVariable<double>("lai");
    QCOMPARE(lai - lai/pow(ph,3)*0.2*0.2*(3.*ph - 2.*0.2), leafArea->aboveHeight(0.2));
}

void TestArea::testCalcEffectiveAreaAbove() {
    const double height = 0.2;
    const double hour = 14.;

    clock()->doTick(hour);
    cout << "sinb equals " << calendar->pullVariable<double>("sinb") << "\n";
    cout << "leaf area above equals " << leafArea->aboveHeight(height) << "\n";

    LightComponents eaa = leafArea->calcEffectiveAreaAbove(height);
    QVERIFY(fabs(eaa[Diffuse] - 1.2544) < 1e-3);
    QVERIFY(fabs(eaa[DirectDirect] - 1.3847) < 1e-3);
    QVERIFY(fabs(eaa[DirectTotal] - 1.2385) < 1e-3);
}


void TestArea::testCalcAbsorptionInShade() {
    const double hour = 14.;
    clock()->doTick(hour);

    LightComponents eaa;
    eaa[Diffuse] = 1.3824;
    eaa[DirectDirect] = 1.5260;
    eaa[DirectTotal] = 1.3649;
    LightComponents absorbed = leafArea->calcAbsorptionInShade(eaa);
    QVERIFY(fabs(absorbed[Diffuse] - 21.4000) < 1e-3);
    QVERIFY(fabs(absorbed[DirectDirect] - 10.3296) < 1e-3);
    QVERIFY(fabs(absorbed[DirectTotal] - 12.8169) < 1e-3);
}

void TestArea::testCalcPhotosynthesisInShade() {
    const double hour = 14.;
    clock()->doTick(hour);

    cout << "temperature equals " << weather->pullVariable<double>("Tday") << "\n";
    cout << "efficiency amax equals "
            << lightUseEfficiency->pullVariable<double>("efficiency") << " "
            << assimilationMax->pullVariable<double>("amax") << "\n";

    LightComponents eaa;
    eaa[Diffuse] = 1.3824;
    eaa[DirectDirect] = 1.5260;
    eaa[DirectTotal] = 1.3649;
    PhotosyntheticRate psInShade = leafArea->calcPhotosynthesisInShade(eaa);
    QVERIFY(fabs(psInShade.absorption() - 23.8873) < 1e-3);
    QVERIFY(fabs(psInShade.assimilation() - 0.5944) < 1e-3);
}

void TestArea::testCalcPhotosynthesisInSun() {
    const double hour = 14.;
    clock()->doTick(hour);

    LightComponents eaa;
    eaa[Diffuse] = 1.3824;
    eaa[DirectDirect] = 1.5260;
    eaa[DirectTotal] = 1.3649;

    PhotosyntheticRate psInShade = leafArea->calcPhotosynthesisInShade(eaa);
    PhotosyntheticRate psInSun = leafArea->calcPhotosynthesisInSun(psInShade);
    QVERIFY(fabs(psInSun.absorption() - 72.4557) < 1e-3);
    QVERIFY(fabs(psInSun.assimilation() - 0.7838) < 1e-3);
}

void TestArea::testCalcPhotosynthesisInTotal() {
    const double hour = 14.;
    clock()->doTick(hour);

    LightComponents eaa;
    eaa[Diffuse] = 1.3824;
    eaa[DirectDirect] = 1.5260;
    eaa[DirectTotal] = 1.3649;

    PhotosyntheticRate psInShade = leafArea->calcPhotosynthesisInShade(eaa);
    PhotosyntheticRate psInSun = leafArea->calcPhotosynthesisInSun(psInShade);
    PhotosyntheticRate psTotal = leafArea->calcPhotosynthesisTotal(eaa, psInShade, psInSun);
    cout << "*** " << psTotal.absorption() << " " << psTotal.assimilation() <<" ***\n";
    QVERIFY(fabs(psTotal.absorption() - 34.4462) < 1e-3);
    QVERIFY(fabs(psTotal.assimilation() - 0.6356) < 1e-3);
}


