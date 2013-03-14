#ifndef TEST_SIMULATION_MAKER_FROM_FILE_H
#define TEST_SIMULATION_MAKER_FROM_FILE_H

#include <usbase/test/autotest.h>

namespace UniSim {
	class Simulation;	
}

class TestSimulationMakerFromFile : public QObject
{
    Q_OBJECT
private slots:
    /*
    void testCommonElement();
    void testModelInstances();
    void testModelsFromFileOneLevel();
    void testModelsFromFileTwoLevels();
    void testModelsFromFileCondensed();
    void testModelsFromFileThreeLevels();
    void testModelsAndParametersFromFile();
    void testModelsAndParametersFromFileNoParent();
    */
    void testModelsAndParametersFromFileSame();
    /*
    void testModelsAndParametersFromFileCondensed();
    void testModelsFromCondensedFileParameterKey();
    void testModelsFromCondensedFileParameter2Keys();
    void testVariableParameter();
    */
private:
    void createSimulation(QString fileName);
    QString filePath(QString fileName) const;
    UniSim::Simulation *sim;
};

DECLARE_TEST(TestSimulationMakerFromFile)

#endif
