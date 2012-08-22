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
#include "../simulation.h"
#include "../simulation_maker.h"
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

    simulation = 0;
	SimulationMaker maker;
	bool modelOk = false;
	try {
        simulation = maker.parse(local::testFilePath());
        UniSim::setSimulationObject(simulation);
        modelOk = true;
	}
	catch (const Exception &ex) {
        delete simulation;
        simulation = 0;
		QWARN(qPrintable(ex.message()));
		QVERIFY(false);
	}
}

void TestSimulationMaker::cleanupTestCase()
{
    delete simulation;
    simulation = 0;
}


void TestSimulationMaker::testSimulation()
{
    QVERIFY(simulation);
    QCOMPARE(simulation->children().size(), 7);
    QVERIFY(simulation->parent()==0);
	
    Model *butterfly = seekOneDescendant<Model*>("butterfly", 0);
	QVERIFY(butterfly);
}

void TestSimulationMaker::testModel()
{
    QList<Model*> models = seekDescendants<Model*>("butterfly", 0);
    QCOMPARE(models.size(), 1);
    Model *butterfly = models[0];
    QVERIFY(butterfly);
    QVERIFY(butterfly->parent() == simulation);
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
        QCOMPARE(larva->pullValue<int>("k"), 10);
        QCOMPARE(larva->pullValue<double>("duration"), 2.3);
	}
	catch (const Exception &ex) {
		QWARN(qPrintable(ex.message()));
		QVERIFY(false);
	}
	
}
