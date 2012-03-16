#include <iostream>
#include <QFile>
#include <QSettings>
#include <QTextStream>
#include <QtXmlPatterns>
#include <usbase/data_grid.h>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/model.h>
#include <usbase/output.h>
#include <usbase/utilities.h>
#include "../simulation.h"
#include "../simulation_maker.h"
#include "test_simulation_maker_from_file.h"

using namespace UniSim;



void TestSimulationMakerFromFile::testCommonElement() {
    createSimulation("common.xml");
    delete sim;
}

void TestSimulationMakerFromFile::testModelInstances() {
    createSimulation("model_instances.xml");
    Models all = sim->seekDescendants<Model*>("*");
    QCOMPARE(all.size(), 3*(4*4+3)+1);
    QVERIFY(sim->peekOneChild<Model*>("generations(1)"));
    QVERIFY(sim->peekOneDescendant<Model*>("generations(2)/egg"));
    QVERIFY(sim->peekOneDescendant<Model*>("generations(3)/pupa"));
    QVERIFY(sim->peekOneDescendant<Model*>("generations(1)/larva(1)"));
    QVERIFY(sim->peekOneDescendant<Model*>("generations(2)/larva(2)/kidney"));
    QVERIFY(sim->peekOneDescendant<Model*>("generations(3)/larva(4)/ovary(2)"));

    delete sim;
}

void TestSimulationMakerFromFile::testModelsFromFileOneLevel() {
    createSimulation("models_from_file.xml");
    Models all = sim->seekDescendants<Model*>("*");
    QCOMPARE(all.size(), 2*4+1+1);
    QVERIFY(sim->peekOneDescendant<Model*>("A"));
    QVERIFY(sim->peekOneDescendant<Model*>("D/stat"));
    QCOMPARE(sim->seekOneDescendant<Model*>("A")->parameter<int>("farmRealId"),
             474840);
    QCOMPARE(sim->seekOneDescendant<Model*>("D")->parameter<QString>("soilType"),
             QString("Clay"));
    delete sim;
}

void TestSimulationMakerFromFile::testModelsFromFileTwoLevels() {
    createSimulation("models_from_file_nested.xml");

    Models all = sim->seekDescendants<Model*>("*");
    QCOMPARE(all.size(), 1+1+4+7+4);
    QVERIFY(sim->peekOneDescendant<Model*>("landscape/A"));
    QVERIFY(sim->peekOneDescendant<Model*>("landscape/A/Oats"));
    QVERIFY(sim->peekOneDescendant<Model*>("landscape/A/stat"));
    QVERIFY(sim->peekOneDescendant<Model*>("landscape/C/stat"));

    try {
        Model *crop = sim->seekOneDescendant<Model*>("landscape/B/WBarley");
        QCOMPARE(crop->parameter<int>("Area"), 40);
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception. " + ex.message();
        QFAIL(qPrintable(msg));
    }

    delete sim;
}

void TestSimulationMakerFromFile::testModelsFromFileCondensed() {
    createSimulation("models_from_file_condensed.xml");

    Models all = sim->seekDescendants<Model*>("*");
    QCOMPARE(all.size(), 1+1+4*5);
    QVERIFY(sim->peekOneDescendant<Model*>("landscape/A"));
    QVERIFY(sim->peekOneDescendant<Model*>("landscape/A/Oats"));
    QVERIFY(sim->peekOneDescendant<Model*>("landscape/A/stat"));
    QVERIFY(sim->peekOneDescendant<Model*>("landscape/C/stat"));

    Model *crop = sim->peekOneDescendant<Model*>("landscape/B/WBarley");
    QCOMPARE(crop->parameter<int>("Area"), 40);
    crop = sim->peekOneDescendant<Model*>("landscape/D/SBarley");
    QCOMPARE(crop->parameter<int>("Area"), 15);

    delete sim;
}

void TestSimulationMakerFromFile::testModelsFromFileThreeLevels()
{
    createSimulation("models_from_file_nested_twice.xml");
    Models all = sim->seekDescendants<Model*>("*");
//        std::cout << "\n";
//        for (int i = 0; i < all.size(); ++i)
//            std::cout << i << " " << qPrintable(all[i]->fullLabel()) << "\n";
//        std::cout << "\n";
    QCOMPARE(all.size(), 1+1+1+4+7+4+16);
    QVERIFY(sim->peekOneDescendant<Model*>("landscape/A"));
    QVERIFY(sim->peekOneDescendant<Model*>("landscape/A/Oats/Harvest"));
    QVERIFY(sim->peekOneDescendant<Model*>("landscape/A/stat"));
    QVERIFY(sim->peekOneDescendant<Model*>("landscape/C/stat"));

    Model *harvest = sim->peekOneDescendant<Model*>("landscape/A/Oats/Harvest");
    QCOMPARE(harvest->parameter<QDate>("Date"), QDate(2010,8,15));

    delete sim;
//    std::cout << "\n";
//    for (int i = 0; i < all.size(); ++i)
//        std::cout << i << " " << qPrintable(all[i]->fullLabel()) << "\n";
//    std::cout << "\n";
}

void TestSimulationMakerFromFile::testModelsAndParametersFromFile() {
    createSimulation("models_and_parameters_from_files.xml");
    Model *harvest = sim->peekOneDescendant<Model*>("landscape/A/Oats/Harvest");
    QCOMPARE(harvest->parameter<QDate>("Date"), QDate(2010,8,15));
    QCOMPARE(harvest->parameter<int>("Cost"), 350);
    QCOMPARE(harvest->parameter<bool>("IsOrganic"), true);
}

void TestSimulationMakerFromFile::testModelsAndParametersFromFileSame() {
    createSimulation("models_and_parameters_from_files_same.xml");
    Model *farmA = sim->peekOneDescendant<Model*>("landscape/A");
    Model *farmD = sim->peekOneDescendant<Model*>("landscape/D");
    QCOMPARE(farmA->parameter<int>("leakageRate"), 120);
    QCOMPARE(farmD->parameter<int>("leakageRate"), 30);
}

void TestSimulationMakerFromFile::testModelsAndParametersFromFileCondensed() {
    createSimulation("models_and_parameters_from_file_condensed.xml");

    Models all = sim->seekDescendants<Model*>("*");
    QCOMPARE(all.size(), 1+1+4*5);
    QVERIFY(sim->peekOneDescendant<Model*>("landscape/A"));
    QVERIFY(sim->peekOneDescendant<Model*>("landscape/A/Oats"));
    QVERIFY(sim->peekOneDescendant<Model*>("landscape/A/stat"));
    QVERIFY(sim->peekOneDescendant<Model*>("landscape/C/stat"));

    Model *crop = sim->seekOneDescendant<Model*>("landscape/B/WBarley");
    QCOMPARE(crop->parameter<int>("Area"), 40);
    QCOMPARE(crop->parameter<int>("numFields"), 4);
    crop = sim->seekOneDescendant<Model*>("landscape/D/SBarley");
    QCOMPARE(crop->parameter<int>("Area"), 15);
    QCOMPARE(crop->parameter<int>("numFields"), 5);

    delete sim;
}

void TestSimulationMakerFromFile::testModelsFromCondensedFileParameterKey() {
    createSimulation("models_from_condensed_file_parameter_key.xml");

    Models all = sim->seekDescendants<Model*>("*");
    QCOMPARE(all.size(), 1+1+4*5);

    Model *crop = sim->seekOneDescendant<Model*>("landscape/C/WBarley");
    QCOMPARE(crop->parameter<int>("nitrogenNorm"), 130);

    crop = sim->seekOneDescendant<Model*>("landscape/D/SBarley");
    QCOMPARE(crop->parameter<int>("nitrogenNorm"), 90);

    delete sim;
}

void TestSimulationMakerFromFile::testModelsFromCondensedFileParameter2Keys() {
    createSimulation("models_from_condensed_file_parameter_2_keys.xml");

    Models all = sim->seekDescendants<Model*>("*");
    QCOMPARE(all.size(), 1+1+4*5);

    Model *crop = sim->seekOneDescendant<Model*>("landscape/C/WBarley");
    QCOMPARE(crop->parameter<int>("nitrogenNorm"), 130);

    crop = sim->seekOneDescendant<Model*>("landscape/D/SBarley");
    QCOMPARE(crop->parameter<int>("nitrogenNorm"), 90);

    crop = sim->seekOneDescendant<Model*>("landscape/B/Oats");
    QCOMPARE(crop->parameter<int>("nitrogenNorm"), 125);

    delete sim;
}

void TestSimulationMakerFromFile::testVariableParameter() {
    createSimulation("variable_parameter.xml");
    sim->execute();
    delete sim;

    QString fileName = "test_variable_parameter.txt";
    QDir dir = FileLocations::location(FileLocationInfo::Output);
    QString filePath = dir.absolutePath() + "/" + fileName;
    DataGrid data(filePath);
    for (int i = 0; i < data.rowNumber(); ++i) {
        QCOMPARE(data.cell(i,2), data.cell(i,5));
    }
}

 void TestSimulationMakerFromFile::createSimulation(QString fileName) {
    SimulationMaker maker;
    try {
        sim = maker.parse(filePath(fileName));
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception. " + ex.message();
        QFAIL(qPrintable(msg));
    }
}

QString TestSimulationMakerFromFile::filePath(QString fileName) const {
    QDir uniSimDir = QDir("../..");
    return uniSimDir.absolutePath() +
            "/src/library/usengine/test/input/simulation_maker/" +
            fileName;
}
