/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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
    dotOption[SVG] = "svg";
    fileType[Postscript] = "eps";
    fileType[PNG] = "png";
    fileType[Dot] = "dot";
    fileType[SVG] = "svg";
}

QString GraphGenerator::dotCommand() {
    QDir dir = FileLocations::location(FileLocationInfo::DotTool);
    QString command = dir.absolutePath() + "/dot.exe";
    if (!QFile::exists(command))
        command = dir.absolutePath() + "/bin/dot.exe";
    if (!QFile::exists(command)) {
        QString msg =
                "Could not find the program \"dot.exe\" of the Graphviz package to draw the model diagram. "
                "The program was not found here: '" + command + "'\n\n"
                "Set the correct path through the File|Locations menu. The correct path should be something like:\n\n'C:/Program Files/Graphviz2.26.3/bin'"
                "\n\nor\n\n"
                "'C:/Program Files/UniSim-1-29/Graphviz2.26.3/bin'\n\n"
                "Alternatively, you may have to download Graphviz from www.graphviz.org.";
        throw Exception(msg);
    }
    return "\"" + command + "\"";
}

QString GraphGenerator::outputFilePath(OutputFormat format) {
    QDir dir = FileLocations::location(FileLocationInfo::Temporary);
    QString path = dir.absolutePath() + "/" + simulation->objectName() + "." + fileType[format];
    return path;
}

// Denne funktion kalder programmet dot.exe
// Den starter en process, som selv melder tilbage når diagrammet er genereret
// Diagrammet ligger i en PNG fil som derefter vises på skærmen. Nå...
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

// Denne funktion (writeDotFile) skriver en tekstfil, som bagefter læses af programmet dot.exe
// Dot er et open-source program, en del af Graphviz
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
        if (!(*mo)->hide())
            writeModel(&f, simulation, *mo, 0);
	}
	f.write("\n}\n");
}

namespace {

    QString nodeId(QObject *node) {
        QString nodeId = node->objectName();
        return nodeId.remove("(").remove(")");
    }

    void writeNode(QTextStream *sink, QObject *node, int number)
	{
        QString className = node->metaObject()->className();
        if (className.contains("Anonymous"))
            className.clear();
        *sink << nodeId(node) << number << "[label=\"" << className;
        if (nodeId(node) != "anonymous") {
            if (!className.isEmpty())
                *sink << "\\n";
            *sink << node->objectName();
        }
		*sink << "\"];";
	}
	
}	

//Limit the number!!
void GraphGenerator::writeModel(QFile *f, QObject *parent, QObject *child, int parentNumber)
{
	QString statement;
        QTextStream sink(&statement);

    int myNumber = ++nodeNumber;
	writeNode(&sink, parent, parentNumber);
	writeNode(&sink, child, myNumber);
    sink << nodeId(parent) << parentNumber << "->" << nodeId(child) << myNumber << ";\n";
			
	f->write(qPrintable(statement));

	for (QList<QObject*>::const_iterator ch = child->children().begin(); ch != child->children().end(); ++ch) {
		Model *model = dynamic_cast<Model*>(*ch);
        if (model && ! model->hide())
            writeModel(f, child, *ch, myNumber);
	}
}

