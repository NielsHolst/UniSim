#include <iostream>
#include <usbase/clock.h>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/utilities.h>
#include <usengine/simulation.h>
#include <usengine/simulation_maker.h>
#include "../area.h"
#include "../photosynthesis.h"
#include "../plant.h"
#include "../light_components.h"
#include "../time_slice.h"
#include "test_photosynthesis.h"

using std::cout;
using namespace UniSim;
using namespace intercom;

void TestPhotosynthesis::initTestCase()
{
    QDir dir = FileLocations::location(FileLocations::Models);
    QString path = dir.absolutePath();

    SimulationMaker *maker = new SimulationMaker;
    try {
        QDir dir = FileLocations::location(FileLocations::Plugins);
        QString filePath = dir.absolutePath() + "/intercom_models/test/test_area_canopy_layer.xml";
        simulation = maker->parse(filePath);
        photosynthesis = seekOneDescendant<Photosynthesis*>("*", simulation);
        timeSlices = seekDescendants<TimeSlice*>("*", photosynthesis);
        QCOMPARE(timeSlices.size(), 3);
        plants = seekDescendants<Plant*>("*", simulation);
        QCOMPARE(plants.size(), 1);
        reset();
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.message()));
    }
    delete maker;
}

void TestPhotosynthesis::reset() {
    Model *calendar, *weather, *field;
    calendar = seekOneDescendant<Model*>("calendar", simulation);
    weather = seekOneDescendant<Model*>("weather", simulation);
    field = seekOneDescendant<Model*>("field", simulation);
    calendar->deepReset();
    weather->deepReset();
    field->deepReset();

    calendar->deepUpdate();
    weather->deepUpdate();
    //field->deepUpdate();
}


void TestPhotosynthesis::testCalculate() {
    PhotosyntheticRate psRate = photosynthesis->calculate();
    cout << " TestPhotosynthesis::testCalculate psRate = " << psRate.absorption() << " " << psRate.assimilation() << "\n";
}



void TestPhotosynthesis::cleanupTestCase()
{
    delete simulation;
}

