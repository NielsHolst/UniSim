#include <iostream>
#include <usbase/clock.h>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/utilities.h>
#include <usengine/simulation.h>
#include <usengine/simulation_maker.h>
#include "../area.h"
#include "../canopy_layer.h"
#include "../plant.h"
#include "../light_components.h"
#include "test_canopy_layer.h"

using std::cout;
using namespace UniSim;
using namespace intercom;

void TestCanopyLayer::initTestCase()
{
    QDir dir = FileLocations::location(FileLocations::Models);
    QString path = dir.absolutePath();

    SimulationMaker *maker = new SimulationMaker;
    try {
        QDir dir = FileLocations::location(FileLocations::Plugins);
        QString filePath = dir.absolutePath() + "/intercom_models/test/test_area_canopy_layer.xml";
        simulation = maker->parse(filePath);
        plant = seekOneDescendant<Model*>("cirsium", simulation);
        canopyLayers = plant->seekChildren<CanopyLayer*>("*");
        QCOMPARE(canopyLayers.size(), 5);
        reset();
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.message()));
    }
    delete maker;
}

void TestCanopyLayer::reset() {
    Model *weather, *field;
    calendar = seekOneDescendant<Model*>("calendar", simulation);
    weather = seekOneDescendant<Model*>("weather", simulation);
    field = seekOneDescendant<Model*>("field", simulation);
    calendar->deepReset();
    weather->deepReset();
    field->deepReset();

    calendar->deepUpdate();
    weather->deepUpdate();
}


void TestCanopyLayer::cleanupTestCase()
{
    delete simulation;
}

void TestCanopyLayer::testCalcEffectiveAreaAbove() {
    const double height = 0.2;
    const double hour = 14.;

    clock()->doTick(hour);
    cout << "sinb equals " << calendar->pullVariable("sinb") << "\n";
    LightComponents eaa = canopyLayers[0]->calcEffectiveAreaAbove(height);

    QList<Area*> areas = seekDescendants<Area*>("*", plant);
    for (int i = 0; i < areas.size(); ++i)
        cout << qPrintable(areas[i]->parent()->objectName()) << " area above equals " << areas[i]->aboveHeight(height) << "\n";

    QVERIFY(fabs(eaa[Diffuse] - 1.3824) < 1e-3);
    QVERIFY(fabs(eaa[DirectDirect] - 1.5260) < 1e-3);
    QVERIFY(fabs(eaa[DirectTotal] - 1.3649) < 1e-3);
}

