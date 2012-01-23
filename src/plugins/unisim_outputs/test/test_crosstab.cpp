#include <usbase/model.h>
#include <usengine/simulation.h>
#include <usengine/simulation_maker.h>
#include "test_crosstab.h"

using namespace UniSim;

void TestCrosstab::test3Levels() {
    QString filename = filePath("models_from_file_nested_twice.xml");
    SimulationMaker maker;
    Simulation * sim = 0;
    try {
        sim = maker.parse(filename);
        sim->execute();
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception. " + ex.message();
        QFAIL(qPrintable(msg));
    }
    Models all = sim->seekDescendants<Model*>("*");
    QCOMPARE(all.size(), 1+1+1+4+7+4+16);


}

QString TestCrosstab::filePath(QString fileName) const {
    QDir uniSimDir = QDir("../..");
    return uniSimDir.absolutePath() +
            "/src/library/usengine/test/input/simulation_maker/" +
            fileName;
}

