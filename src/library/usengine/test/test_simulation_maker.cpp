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
#include "../xml_expander.h"
#include "../xy_state_variables.h"
#include "test_simulation_maker.h"

using namespace UniSim;

//! \cond
namespace local
{
	QString testFolder() {
	    QDir dir = FileLocations::location(FileLocations::Temporary);
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
    QCOMPARE(_simulation->children().size(), 6);
	QVERIFY(_simulation->parent()==0);
	
    Model *butterfly = findOne<Model*>("butterfly");
	QVERIFY(butterfly);
}

void TestSimulationMaker::testModel()
{
    QList<Model*> models = find<Model*>("butterfly");
    QCOMPARE(models.size(), 1);
    Model *butterfly = models[0];
    QVERIFY(butterfly);
	QVERIFY(butterfly->parent() == _simulation);
    QCOMPARE(butterfly->metaObject()->className(), "test::LifeCycle");
	QCOMPARE(butterfly->metaObject()->superClass()->className(), "UniSim::Model");
	QCOMPARE(butterfly->parent()->metaObject()->className(), "UniSim::Simulation");
	QCOMPARE(butterfly->children().size(), 4);
}

void TestSimulationMaker::testParameters()
{
    QList<Model*> models = find<Model*>("butterfly");
    QCOMPARE(models.size(), 1);
    Model *butterfly = models[0];
    QVERIFY(butterfly);

    models = UniSim::findChildren<Model*>("larva", butterfly);
    QCOMPARE(models.size(), 1);
    Model *larva = models[0];
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

void writeQuery(QString q, int number)
{
    QXmlQuery query;
    query.setQuery("doc('" + local::testFilePath() +"')" + q);
  	
    QString queryResultFilePath = local::testFolder() + "/test_UniSim_query_" + QString::number(number) + ".xml";
  	QFile file(queryResultFilePath);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QCOMPARE(file.error(), QFile::NoError);

  	QXmlSerializer serializer(query, &file);
  	query.evaluateTo(&serializer);
}

void TestSimulationMaker::testXmlQuery()
{
	writeQuery("/simulation/model", 1);
	writeQuery("/simulation//model[@type='LifeCycle']", 2);
	writeQuery("/simulation/model[@name='wasp']/model[@name='egg']/parameter[@name='k']", 3);
	writeQuery("/simulation/output[@name='butterflyOutput']/"
	               "presentation[@name='butterflyPlot']/xyvariables[@name='butterflyXY']", 4);
	
	QXmlQuery query;
	query.setQuery("<e/>, 1, 'two'");
	QXmlResultItems result;
	
	if (query.isValid()) {
		query.evaluateTo(&result);
		QXmlItem item(result.next());
		while (!item.isNull()) {
			// use item
			item = result.next();
		}
		if (result.hasError())
	 /* Runtime error! */;
	}
}
	 
