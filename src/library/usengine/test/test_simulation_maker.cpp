#include <iostream>
#include <QFile>
#include <QSettings>
#include <QTextStream>
#include <QtXmlPatterns>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/model.h>
#include <usbase/output.h>
#include <usbase/utilities.h>
#include "../output_destination_maker_file.h"
#include "../simulation.h"
#include "../simulation_maker.h"
#include "../xy_state_variables.h"
#include "test_simulation_maker.h"

using namespace UniSim;

//! \cond
namespace local
{
	QString testFolder() {
	    QDir dir = FileLocations::location(FileLocationInfo::Temporary);
	    return dir.absolutePath();
	}
	
	QString testFilePath() {
		return testFolder() + "/test.xml";
	}
}
//! \endcond
	
void TestSimulationMaker::initTestCase()
{	
    writeStandardTestFile(local::testFilePath());

	_simulation = 0;
	SimulationMaker maker;
	bool modelOk = false;
	try {
        _simulation = maker.parse(local::testFilePath());
        UniSim::setSimulationObject(_simulation);
        modelOk = true;
	}
	catch (const Exception &ex) {
		delete _simulation;
		_simulation = 0;
		QWARN(qPrintable(ex.message()));
		QVERIFY(false);
	}
}

void TestSimulationMaker::cleanupTestCase()
{
	delete _simulation;
	_simulation = 0;
}

void TestSimulationMaker::testSimulation()
{
	QVERIFY(_simulation);
    QCOMPARE(_simulation->children().size(), 7);
	QVERIFY(_simulation->parent()==0);
	
    Model *butterfly = seekOneDescendant<Model*>("butterfly", 0);
	QVERIFY(butterfly);
}

void TestSimulationMaker::testModel()
{
    QList<Model*> models = seekDescendants<Model*>("butterfly", 0);
    QCOMPARE(models.size(), 1);
    Model *butterfly = models[0];
    QVERIFY(butterfly);
	QVERIFY(butterfly->parent() == _simulation);
    QCOMPARE(butterfly->metaObject()->className(), "test::LifeCycle");
	QCOMPARE(butterfly->metaObject()->superClass()->className(), "UniSim::Model");
	QCOMPARE(butterfly->parent()->metaObject()->className(), "UniSim::Simulation");
    QCOMPARE(butterfly->seekChildren<Model*>("*").size(), 4);
}

void TestSimulationMaker::testParameters()
{
    Model *butterfly = seekOneDescendant<Model*>("butterfly", 0);
    QVERIFY(butterfly);

    Model *larva = UniSim::seekOneChild<Model*>("larva", butterfly);
    QVERIFY(larva);

    try {
		QCOMPARE(larva->parameter<int>("k"), 10);	
		QCOMPARE(larva->parameter<double>("duration"), 2.3);	
	}
	catch (const Exception &ex) {
		QWARN(qPrintable(ex.message()));
		QVERIFY(false);
	}
	
}

void dumpVariables(const XYStateVariables::Variables &var) {
	QString s;
	QTextStream text(&s);
	for (int i = 0;i < var.size(); ++i) {
        text << i << " " << var[i].label << " " << var[i].stateName
		<< " " << var[i].model->objectName() << " " << *(var[i].value) << "\n";
	}
	std::cout << qPrintable(s);
}

void TestSimulationMaker::testOutputOneXY()
{
/*	QList<Output*> outputs;
	Output *output;
	QList<XYStateVariables*> xys;
	XYStateVariables *xy;
	
    outputs = UniSim::find<Output*>("butterflyPlot");
	QCOMPARE(outputs.size(), 1);
	
	output = outputs[0];
	QVERIFY(output->parent() == _simulation);

	
    xys = UniSim::findChildren<XYStateVariables*>("*", output);
	QCOMPARE(xys.size(), 1);
	
	xy = xys[0];
	XYStateVariables::Variables 
		x =  xy->xVariables(), 
		y = xy->yVariables();
	QCOMPARE(x.size(), 1);
	dumpVariables(y);
	QCOMPARE(y.size(), 5);
*/
}

void TestSimulationMaker::testOutputManyXY()
{/*
	QList<Output*> outputs;
	Output *output;
	QList<XYStateVariables*> xys;
	XYStateVariables *xy;
	
    outputs = UniSim::find<Output*>("butterflyPhasePlot");
	QCOMPARE(outputs.size(), 1);
	
	output = outputs[0];
	QVERIFY(output->parent() == _simulation);

	
    xys = UniSim::findChildren<XYStateVariables*>("*", output);
    QCOMPARE(xys.size(), 2);
	
	xy = xys[1];
	XYStateVariables::Variables 
		x =  xy->xVariables(), 
		y = xy->yVariables();
	QCOMPARE(x.size(), 1);
	QCOMPARE(y.size(), 1);
*/
}

void TestSimulationMaker::testCommonElement() {
    QString filename = filePath("common.xml");
    SimulationMaker maker;
    Simulation * sim = 0;
    try {
        sim = maker.parse(filename);
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception. " + ex.message();
        QFAIL(qPrintable(msg));
    }
    delete sim;
}

void TestSimulationMaker::testModelInstances() {
    QString filename = filePath("model_instances.xml");
    SimulationMaker maker;
    Simulation * sim = 0;
    try {
        sim = maker.parse(filename);
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception. " + ex.message();
        QFAIL(qPrintable(msg));
    }
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

void TestSimulationMaker::testModelsFromFileOneLevel() {
    QString filename = filePath("models_from_file.xml");
    SimulationMaker maker;
    Simulation * sim = 0;
    try {
        sim = maker.parse(filename);
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception. " + ex.message();
        QFAIL(qPrintable(msg));
    }
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

void TestSimulationMaker::testModelsFromFileTwoLevels() {
    QString filename = filePath("models_from_file_nested.xml");
    SimulationMaker maker;
    Simulation * sim = 0;
    try {
        sim = maker.parse(filename);
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception. " + ex.message();
        QFAIL(qPrintable(msg));
    }

    Models all = sim->seekDescendants<Model*>("*");
    std::cout << "\n";
    for (int i = 0; i < all.size(); ++i)
        std::cout << i << " " << qPrintable(all[i]->fullLabel()) << "\n";
    std::cout << "\n";
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

void TestSimulationMaker::testModelsFromFileCondensed() {
    QString filename = filePath("models_from_file_condensed.xml");
    SimulationMaker maker;
    Simulation * sim = 0;
    try {
        sim = maker.parse(filename);
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception. " + ex.message();
        QFAIL(qPrintable(msg));
    }

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

void TestSimulationMaker::testModelsFromFileThreeLevels()
{
    QString filename = filePath("models_from_file_nested_twice.xml");
    SimulationMaker maker;
    Simulation * sim = 0;
    try {
        sim = maker.parse(filename);
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception. " + ex.message();
        QFAIL(qPrintable(msg));
    }
    Models all = sim->seekDescendants<Model*>("*");
    QCOMPARE(all.size(), 1+1+4+7+4+16);
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

void TestSimulationMaker::testModelsAndParametersFromFile() {
    QString filename = filePath("models_and_parameters_from_files.xml");
    SimulationMaker maker;
    Simulation * sim = 0;
    try {
        sim = maker.parse(filename);
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception. " + ex.message();
        QFAIL(qPrintable(msg));
    }

    Model *harvest = sim->peekOneDescendant<Model*>("landscape/A/Oats/Harvest");
    QCOMPARE(harvest->parameter<QDate>("Date"), QDate(2010,8,15));
    QCOMPARE(harvest->parameter<int>("Cost"), 350);
    QCOMPARE(harvest->parameter<bool>("IsOrganic"), true);
}

void TestSimulationMaker::testModelsAndParametersFromFileCondensed() {
    QString filename = filePath("models_and_parameters_from_file_condensed.xml");
    SimulationMaker maker;
    Simulation * sim = 0;
    try {
        sim = maker.parse(filename);
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception. " + ex.message();
        QFAIL(qPrintable(msg));
    }

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

void TestSimulationMaker::testModelsFromCondensedFileParameterKey() {
    QString filename = filePath("models_from_condensed_file_parameter_key.xml");
    SimulationMaker maker;
    Simulation * sim = 0;
    try {
        sim = maker.parse(filename);
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception. " + ex.message();
        QFAIL(qPrintable(msg));
    }

    Models all = sim->seekDescendants<Model*>("*");
        std::cout << "\n";
        for (int i = 0; i < all.size(); ++i)
            std::cout << i << " " << qPrintable(all[i]->fullLabel()) << "\n";
        std::cout << "\n";

    QCOMPARE(all.size(), 1+1+4*5);

    Model *crop = sim->seekOneDescendant<Model*>("landscape/C/WBarley");
    QCOMPARE(crop->parameter<int>("nitrogenNorm"), 130);

    crop = sim->seekOneDescendant<Model*>("landscape/D/SBarley");
    QCOMPARE(crop->parameter<int>("nitrogenNorm"), 90);

    delete sim;
}

QString TestSimulationMaker::filePath(QString fileName) const {
    QDir uniSimDir = QDir("../..");
    return uniSimDir.absolutePath() +
            "/src/library/usengine/test/input/simulation_maker/" +
            fileName;
}
