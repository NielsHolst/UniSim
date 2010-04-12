#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/utilities.h>
#include <usengine/simulation.h>
#include <usengine/simulation_maker.h>
#include "../area.h"
#include "../light_components.h"
#include "test_area.h"

using namespace UniSim;
using namespace intercom;

void TestArea::initTestCase()
{
    QDir dir = FileLocations::location(FileLocations::Models);
    QString path = dir.absolutePath();
/*
    SimulationMaker *maker = new SimulationMaker;
    try {
        QDir dir = FileLocations::location(FileLocations::Plugins);
        QString filePath = dir.absolutePath() + "/intercom_models/test/test_area.xml";
        simulation = maker->parse(filePath);
        area = seekOneDescendant<Area*>("leaves", simulation);
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.message()));
    }
    delete maker;*/
}

void TestArea::cleanupTestCase()
{
    //delete simulation;
}

void TestArea::testWeightedAreaAboveLayer() {
    /*
    const double height = 0.2;
    LightComponents waal = area->weightedAreaAboveLayer(height);

    QCOMPARE(waal[Diffuse], 999.);
    QCOMPARE(waal[DirectDirect], 999.);
    QCOMPARE(waal[DirectTotal], 999.);*/
}

void TestArea::testLightUseInShade(){
}

void TestArea::testLightUseInSun() {
}

void TestArea::testLightUseTotal() {

}

