#include <iostream>
#include <usbase/clock.h>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/utilities.h>
#include <usengine/simulation.h>
#include <usengine/simulation_maker.h>
#include "../area.h"
#include "../plant.h"
#include "../light_components.h"
#include "test_plant.h"

using std::cout;
using namespace UniSim;
using namespace intercom;

void TestPlant::initTestCase()
{
    QDir dir = FileLocations::location(FileLocationInfo::Models);
    QString path = dir.absolutePath();

    SimulationMaker *maker = new SimulationMaker;
    try {
        QDir dir = FileLocations::location(FileLocationInfo::Plugins);
        QString filePath = dir.absolutePath() + "/intercom_models/test/test_area.xml";
        simulation = maker->parse(filePath);
        plant = seekOneDescendant<Plant*>("cirsium", simulation);
        height = seekOneDescendant<Model*>("height", simulation);
        calendar = seekOneDescendant<Model*>("calendar", simulation);
        reset();
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.message()));
    }
    delete maker;
}

void TestPlant::reset() {
    Model *calendar, *weather, *field;
    calendar = seekOneDescendant<Model*>("calendar", simulation);
    weather = seekOneDescendant<Model*>("weather", simulation);
    field = seekOneDescendant<Model*>("field", simulation);
    calendar->deepReset();
    weather->deepReset();
    field->deepReset();
}


void TestPlant::cleanupTestCase()
{
    delete simulation;
}

void TestPlant::testWeightedAreaAboveLayer() {
    const double height = 0.2;
    const double hour = 14.;

    Area* leafArea = seekOneDescendant<Area*>("leaves/area", plant);
    Area* stemArea = seekOneDescendant<Area*>("stem/area", plant);

    clock()->emitToll(this, hour);
    cout << "sinb equals " << calendar->state("sinb") << "\n";
    cout << "leaf area above equals " << leafArea->aboveHeight(height) << "\n";
    cout << "stem area above equals " << stemArea->aboveHeight(height) << "\n";

    LightComponents waal = plant->weightedAreaAboveLayer(height);

    QVERIFY(fabs(waal[Diffuse] - 1) < 1e-3);
    QVERIFY(fabs(waal[DirectDirect] - 1) < 1e-3);
    QVERIFY(fabs(waal[DirectTotal] - 1) < 1e-3);
}
