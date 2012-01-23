#ifndef TEST_MODEL_PARSER_H
#define TEST_MODEL_PARSER_H

#include <usbase/test/autotest.h>

namespace UniSim {
	class Simulation;	
}

class TestSimulationMaker : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void testSimulation();
    void testModel();
    void testParameters();
    void testCommonElement();
    void testModelInstances();
    void testModelsFromFileOneLevel();
    void testModelsFromFileTwoLevels();
    void testModelsFromFileCondensed();
    void testModelsFromFileThreeLevels();
    void testModelsAndParametersFromFile();
    void testModelsAndParametersFromFileCondensed();
    void testModelsFromCondensedFileParameterKey();
    void testModelsFromCondensedFileParameter2Keys();
    void cleanupTestCase();
private:
    QString filePath(QString fileName) const;
    UniSim::Simulation *simulation;
};

DECLARE_TEST(TestSimulationMaker)

#endif
