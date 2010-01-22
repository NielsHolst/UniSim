/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include <QTextStream>
#include <QTimer>
#include <QXmlStreamReader>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/model.h>
#include <usbase/utilities.h>
#include <usengine/simulation.h>
#include "graph_generator.h"

using namespace UniSim;

GraphGenerator::GraphGenerator(UniSim::Simulation *simulation, QString outputPath)
        : _simulation(simulation), _outputPath(outputPath)
{	
     QDir dir = UniSim::FileLocations::location(UniSim::FileLocations::DotTool);
    _dotToolPath = dir.absolutePath() + "/dot.exe";
	
	_dotFilePath = _outputPath;
	_dotFilePath.replace(".png", ".dot");
}

GraphGenerator::~GraphGenerator() 
{
}

QProcess* GraphGenerator::generate()
{
	if (!QFile(_dotToolPath).exists())
		throw UniSim::Exception("Could not find dot program to draw graph:\n'" + _dotToolPath + "'");
	generateDot();
	return generatePng();	
}

QProcess* GraphGenerator::generatePng()
{
	QProcess *process = new QProcess;
    QString command = "\"" + _dotToolPath + "\"";
    QStringList args;
    args << "-Tpng" << _dotFilePath << "-o" << _outputPath;
    process->start(command, args);
	return process;
}

void GraphGenerator::generateDot()
{
	QFile f(_dotFilePath);
	if (!f.open(QIODevice::Text | QIODevice::WriteOnly))   
		throw UniSim::Exception("Could not open output file to draw graph:\n'" + _dotFilePath + "'");

	_nodeNumber = 0;
    f.write("digraph G {size=\"16,24\";graph[rankdir=LR];\n");

    Models models = UniSim::findChildren<Model*>("*", _simulation);
    for (Models::const_iterator mo = models.begin(); mo != models.end(); ++mo) {
		writeModel(&f, _simulation, *mo, 0);
	}
	f.write("\n}\n");
}

namespace {
	
	void writeNode(QTextStream *sink, QObject *node, int number)
	{
		QString className = node->metaObject()->className();
        //className.remove("UniSim::");
		*sink << node->objectName() << number << "[label=\"" << className << "\\n";
		if (node->objectName() != "anonymous")
			*sink << node->objectName();
		*sink << "\"];";
	}
	
}	

void GraphGenerator::writeModel(QFile *f, QObject *parent, QObject *child, int parentNumber)
{
	QString statement;
	QTextStream sink(&	statement);

	int myNumber = ++_nodeNumber;
	writeNode(&sink, parent, parentNumber);
	writeNode(&sink, child, myNumber);
	sink << parent->objectName() << parentNumber << "->" << child->objectName() << myNumber << ";\n";
			
	f->write(qPrintable(statement));

	for (QList<QObject*>::const_iterator ch = child->children().begin(); ch != child->children().end(); ++ch) {
		Model *model = dynamic_cast<Model*>(*ch);
		if (model) writeModel(f, child, *ch, myNumber);
	}
}

