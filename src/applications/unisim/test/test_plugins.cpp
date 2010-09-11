#include <iostream>
#include <QDir>
#include <usbase/file_locations.h>
#include <usengine/simulation.h>
#include <usengine/simulation_maker.h>
#include "test_plugins.h"

using namespace UniSim;

namespace local {

    Simulation *sim = 0;

    QDir xmlModelsTestFolder() {
        QDir dir = FileLocations::location(FileLocationInfo::Plugins);
        dir.cdUp();
        dir.cdUp();
        dir.cd("xml");
        dir.cd("models");
        dir.cd("test");
        return dir;
    }

    void runSimulation(QString fileName) {
        QString absFilePath = xmlModelsTestFolder().absolutePath() + "/" + fileName;

        SimulationMaker maker;
        try {
            struct finally {
                ~finally() { delete sim; sim = 0; }
                void touch() {}
            } finale;
            finale.touch();

            sim = maker.parse(absFilePath);
            sim->execute();
        }
        catch (Exception &ex) {
            throw ex;
        }
    }

}

void TestPlugins::testMissingXmlModel() {
    try {
        local::runSimulation("missing_model");
        QString msg = "Should not execute";
        QFAIL(qPrintable(msg));
    }
    catch (Exception &ex) {
    }
}


void TestPlugins::testCorrectXmlModels()
{
    QWARN("Not implemented");
    /*
    QStringList models = QStringList()
        << "ambrosia.xml"
        << "beehive.xml"
        << "grainstore.xml"
        << "intercom.xml"
        << "ess2009.xml";

    for (int i = 0; i < models.size(); ++i) {
        try {
            local::runSimulation(models[i]);
        }
        catch (Exception &ex) {
            QString msg = "Could not run model: " + models[i] +
                          "\n" + ex.message();
            QFAIL(qPrintable(msg));
        }
    }
    */
}
