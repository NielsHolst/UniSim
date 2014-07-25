#include <iostream>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/model.h>
#include <usbase/moments.h>
#include <usbase/parameter.h>
#include <usbase/test_num.h>
#include <usbase/variable.h>
#include <usengine/simulation_maker.h>
#include "../stage.h"
#include "test_stage.h"

using namespace UniSim;

void TestStage::initTestCase() {
    sim = 0;
    output = 0;
}

void TestStage::cleanup() {
    delete sim;
    delete output;
    sim = 0;
    output = 0;
}

void TestStage::testSimulationWithoutGrowth() {
    try {
        executeAndVerify("input", "test_stage_without_growth", 100., 0.);
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.message()));
    }
}

void TestStage::testSimulationWithGrowth() {
    try {
        executeAndVerify("input", "test_stage_with_growth", 700., 600.);
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.message()));
    }
}

void TestStage::testSimulationWithGrowthLimited() {
    try {
        executeAndVerify("input", "test_stage_with_growth_limited", 637.5135, 537.5135);
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.message()));
    }
}

void TestStage::test_1_1_1D_1D() {
    try {
        execute("input/stage_dimensions", "test_stage_1-1_1D-1D");
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.message()));
    }
    double totalInflow = sim->seekOneDescendant<Model*>("egg")->pullValue<double>("initialInflow");
    double totalOutflow = sim->seekOneDescendant<Model*>("chicken")->pullValue<double>("outflowTotal");
    QVERIFY(TestNum::eq(totalInflow, totalOutflow));
}

void TestStage::test_k_k_1D_1D() {
    const char fn[] = "test_stage_k-k_1D-1D";
    try {
        execute("input/stage_dimensions", fn);
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.message()));
    }
    // Check totals
    double totalInflow = sim->seekOneDescendant<Model*>("egg")->pullValue<double>("initialInflow");
    double totalOutflow1 = sim->seekOneDescendant<Model*>("infectedEgg")->pullValue<double>("outflowTotal");
    double totalOutflow2 = sim->seekOneDescendant<Model*>("chicken")->pullValue<double>("outflowTotal");
    QVERIFY(TestNum::eq(totalInflow, totalOutflow1 + totalOutflow2));

    // Check proportions
    createOutputGrid(fn);
    double
        eggDay1 = output->cell(1,1).toDouble(),
        iEggDay1 = output->cell(1,2).toDouble(),
        eggDay2 = output->cell(2,1).toDouble(),
        iEggDay2 = output->cell(2,2).toDouble(),
        proportion = sim->seekOneDescendant<Model*>("egg")->pullValue<double>("phaseOutflowProportion");

    QCOMPARE(eggDay1, (1.-proportion)*totalInflow);
    QCOMPARE(iEggDay1, proportion*totalInflow);

    QCOMPARE(eggDay2, (1.-proportion)*eggDay1);
    QCOMPARE(iEggDay2, iEggDay1 + proportion*eggDay1);
}

void TestStage::test_k_k_1D_2D() {
    try {
        execute("input/stage_dimensions", "test_stage_k-k_1D-2D");
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.message()));
    }
    // Check totals
    double totalInflow = sim->seekOneDescendant<Model*>("egg")->pullValue<double>("initialInflow");
    double totalOutflow1 = sim->seekOneDescendant<Model*>("egg")->pullValue<double>("outflowTotal");
    double totalOutflow2 = sim->seekOneDescendant<Model*>("infectedEgg")->pullValue<double>("outflowTotal");
    double totalOutflow3 = sim->seekOneDescendant<Model*>("infectedEgg")->pullValue<double>("phaseOutflowTotal");
    QVERIFY(TestNum::eq(totalInflow, totalOutflow1 + totalOutflow2 + totalOutflow3));}

void TestStage::test_k_k_2D_1D() {
    try {
        execute("input/stage_dimensions", "test_stage_k-k_2D-1D");
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.message()));
    }
    // Check totals
    double totalInflow = sim->seekOneDescendant<Model*>("egg")->pullValue<double>("initialInflow");
    double totalOutflow1 = sim->seekOneDescendant<Model*>("egg")->pullValue<double>("outflowTotal");
    double totalOutflow2 = sim->seekOneDescendant<Model*>("infectedEgg")->pullValue<double>("outflowTotal");
    double totalOutflow3 = sim->seekOneDescendant<Model*>("resistantEgg")->pullValue<double>("outflowTotal");
    QVERIFY(TestNum::eq(totalInflow, totalOutflow1 + totalOutflow2 + totalOutflow3));
}

void TestStage::test_k_k_2D_2D() {
    try {
        execute("input/stage_dimensions", "test_stage_k-k_2D-2D");
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.message()));
    }
    // Check totals
    double totalInflow = sim->seekOneDescendant<Model*>("susceptible")->pullValue<double>("initialInflow");
    double totalOutflow1 = sim->seekOneDescendant<Model*>("susceptible")->pullValue<double>("outflowTotal");
    double totalOutflow2 = sim->seekOneDescendant<Model*>("exposed")->pullValue<double>("outflowTotal");
    double totalOutflow3 = sim->seekOneDescendant<Model*>("infectious")->pullValue<double>("outflowTotal");
    double totalOutflow4 = sim->seekOneDescendant<Model*>("infectious")->pullValue<double>("phaseOutflowTotal");
    QVERIFY(TestNum::eq(totalInflow, totalOutflow1 + totalOutflow2 + totalOutflow3 + totalOutflow4));
}

void TestStage::test_n_n_2D_2D() {
    try {
        execute("input/stage_dimensions", "test_stage_n-n_2D-2D");
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.message()));
    }
    // Check totals
    double totalInflow = sim->seekOneDescendant<Model*>("susceptibleE")->pullValue<double>("initialInflow");
    double out1 = sim->seekOneDescendant<Model*>("infectiousE")->pullValue<double>("phaseOutflowTotal");
    double out2 = sim->seekOneDescendant<Model*>("infectiousL")->pullValue<double>("phaseOutflowTotal");
    double out3 = sim->seekOneDescendant<Model*>("susceptibleL")->pullValue<double>("outflowTotal");
    double out4 = sim->seekOneDescendant<Model*>("exposedL")->pullValue<double>("outflowTotal");
    double out5 = sim->seekOneDescendant<Model*>("infectiousL")->pullValue<double>("outflowTotal");
    QVERIFY(TestNum::eq(totalInflow, out1+out2+out3+out4+out5));
}

void TestStage::test_wrong_k_Stage() {
    bool excepted = false;
    try {
        execute("input/stage_dimensions", "test_wrong_k_Stage");
    }
    catch (Exception &ex) {
        excepted = true;
        bool correctException = ex.message().contains("does not match the size");
        QVERIFY(correctException);
    }
    QVERIFY(excepted);
}

void TestStage::test_wrong_k_StageAndPhase() {
    bool excepted = false;
    try {
        execute("input/stage_dimensions", "test_wrong_k_StageAndPhase");
    }
    catch (Exception &ex) {
        excepted = true;
        bool correctException = ex.message().contains("does not match the size");
        if (!correctException)
            std::cout << "Exception: " << qPrintable(ex.message());
        QVERIFY(correctException);
    }
    QVERIFY(excepted);
}

void TestStage::test_wrong_phaseK_StageAndPhase() {
    bool excepted = false;
    try {
        execute("input/stage_dimensions", "test_wrong_phaseK_StageAndPhase");
    }
    catch (Exception &ex) {
        excepted = true;
        bool correctException = ex.message().contains("does not match the size");
        QVERIFY(correctException);
    }
    QVERIFY(excepted);
}

void TestStage::test_integration_seir() {
    try {
        execute("input/stage_integration", "seir");
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.message()));
    }
}


void TestStage::execute(QString folder, QString fileBaseName) {
    QDir dir = FileLocations::location(FileLocationInfo::Plugins);
    QString filePath = dir.absolutePath() + "/unisim/test/" + folder + "/" + fileBaseName + ".xml";

    SimulationMaker maker;
    sim = maker.parse(filePath);
    sim->execute();
}

void TestStage::executeAndVerify(QString folder, QString fileBaseName, double sum, double totalGrowth) {
    execute(folder, fileBaseName);
    createOutputGrid(fileBaseName);
    Moments mom;
    mom.append(output->column<double>(0), output->column<double>(1));
    QVERIFY(TestNum::eq(mom.average(), 51., 1e-4));
    QVERIFY(TestNum::eq(mom.variance(), 58.4583, 1e-4));
    QVERIFY(TestNum::eq(mom.sum(), sum, 1e-4));

    double g = 0;
    QVector<double> demands = output->column<double>(3);
    for (int i=0; i < demands.size(); ++i)
        g += demands[i];
    QVERIFY(TestNum::eq(g, totalGrowth, 1e-4));
}

void TestStage::createOutputGrid(QString fileBaseName) {
    QDir dir = FileLocations::location(FileLocationInfo::Output);
    QString filePath = dir.absolutePath() + "/" + fileBaseName + ".txt";
    output = new DataGrid(filePath);
}
