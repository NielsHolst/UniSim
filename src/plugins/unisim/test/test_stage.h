#ifndef UNISIM_TEST_STAGE_H
#define UNISIM_TEST_STAGE_H

#include <usbase/data_grid.h>
#include <usbase/test/autotest.h>
#include <usengine/simulation.h>

namespace UniSim {
    class Simulation;
}

class TestStage : public QObject
{
    Q_OBJECT

private:
    void execute(QString folder, QString fileBaseName);
    void executeAndVerify(QString folder, QString fileBaseName, double sum, double totalGrowth);
    void createOutputGrid(QString fileBaseName);

private slots:
    void initTestCase();
    void cleanup();
    void testSimulationWithoutGrowth();
    void testSimulationWithGrowth();
    void testSimulationWithGrowthLimited();
    void test_1_1_1D_1D();
    void test_k_k_1D_1D();
    void test_k_k_1D_2D();
    void test_k_k_2D_1D();
    void test_k_k_2D_2D();
    void test_n_n_2D_2D();
    void test_wrong_k_Stage();
    void test_wrong_k_StageAndPhase();
    void test_wrong_phaseK_StageAndPhase();
    void test_integration_seir();

private:
    UniSim::Simulation *sim;
    UniSim::DataGrid *output;
};

DECLARE_TEST(TestStage)


#endif
