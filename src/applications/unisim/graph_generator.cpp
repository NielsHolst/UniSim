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

GraphGenerator::GraphGenerator(UniSim::Simulation *simulation_)
        : simulation(simulation_), dotFileOk(false)
{	
    dotOption[Postscript] = "ps";
    dotOption[PNG] = "png";
    fileType[Postscript] = "eps";
    fileType[PNG] = "png";
    fileType[Dot] = "dot";
}

QString GraphGenerator::dotCommand() {
    QDir dir = UniSim::FileLocations::location(UniSim::FileLocationInfo::DotTool);
    QString command = dir.absolutePath() + "/dot.exe";
    if (!QFile::exists(command))
        command = dir.absolutePath() + "/bin/dot.exe";
    if (!QFile::exists(command))
        throw UniSim::Exception("Could not find dot program to draw graph:'" + command + "'");
    return "\"" + command + "\"";
}

QString GraphGenerator::outputFilePath(OutputFormat format) {
    QDir dir = UniSim::FileLocations::location(UniSim::FileLocationInfo::Temporary);
    QString path = dir.absolutePath() + "/" + simulation->objectName() + "." + fileType[format];
    return path;
}

QProcess* GraphGenerator::generate(OutputFormat format)
{
    Q_ASSERT(format != Dot);
    if (!dotFileOk)
        writeDotFile();

    QStringList args;
    args << ("-T" + dotOption[format])
         << outputFilePath(Dot)
         << "-o" << outputFilePath(format);

    QProcess *process = new QProcess;
    process->start(dotCommand(), args);
	return process;
}

void GraphGenerator::writeDotFile()
{
    QString dotFilePath = outputFilePath(Dot);
    QFile f(dotFilePath);
	if (!f.open(QIODevice::Text | QIODevice::WriteOnly))   
        throw Exception("Could not open output file to draw graph:'" + dotFilePath + "'");

    nodeNumber = 0;
    f.write("digraph G {size=\"16,24\";graph[rankdir=LR];\n");

    Models models = seekChildren<Model*>("*", simulation);
    for (Models::const_iterator mo = models.begin(); mo != models.end(); ++mo) {
        writeModel(&f, simulation, *mo, 0);
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

    int myNumber = ++nodeNumber;
	writeNode(&sink, parent, parentNumber);
	writeNode(&sink, child, myNumber);
	sink << parent->objectName() << parentNumber << "->" << child->objectName() << myNumber << ";\n";
			
	f->write(qPrintable(statement));

	for (QList<QObject*>::const_iterator ch = child->children().begin(); ch != child->children().end(); ++ch) {
		Model *model = dynamic_cast<Model*>(*ch);
		if (model) writeModel(f, child, *ch, myNumber);
	}
}

