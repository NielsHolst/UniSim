#include <iostream>
#include <usbase/data_grid.h>
#include <usbase/model.h>
#include <usbase/moments.h>
#include <usbase/test_num.h>
#include <usengine/simulation_maker.h>
#include "../stage.h"
#include "test_stage.h"

using namespace UniSim;

void TestStage::initTestCase() {
    sim = 0;
}

void TestStage::cleanup() {
    delete sim;
    sim = 0;
}

void TestStage::testSimulationWithoutGrowth() {
    executeAndVerify("test_stage_without_growth", 100., 0.);
}

void TestStage::executeAndVerify(QString fileBaseName, double sum, double totalGrowth) {
    execute(fileBaseName + ".xml");

    QDir dir = FileLocations::location(FileLocationInfo::Output);
    QString filePath = dir.absolutePath() + "/" + fileBaseName + ".txt";
    DataGrid output(filePath);

    Moments mom;
    mom.append(output.column<double>(0), output.column<double>(1));
    QVERIFY(TestNum::eq(mom.average(), 51., 1e-4));
    QVERIFY(TestNum::eq(mom.variance(), 58.4583, 1e-4));
    QVERIFY(TestNum::eq(mom.sum(), sum, 1e-4));

    double g = 0;
    QVector<double> demands = output.column<double>(3);
    for (int i=0; i < demands.size(); ++i)
        g += demands[i];
    QVERIFY(TestNum::eq(g, totalGrowth, 1e-4));
}

void TestStage::execute(QString fileName) {
    SimulationMaker maker;
    sim = maker.parse(FileLocationInfo::Plugins, "unisim_models/test/input", fileName);
    sim->execute();
}

void TestStage::testSimulationWithGrowth() {
    executeAndVerify("test_stage_with_growth", 700., 600.);
}

void TestStage::testSimulationWithGrowthLimited() {
    executeAndVerify("test_stage_with_growth_limited", 637.5135, 537.5135);
}
