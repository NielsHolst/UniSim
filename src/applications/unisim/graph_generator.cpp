/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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
#include <usbase/named_object.h>
#include <usbase/ref.h>
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
#ifdef Q_OS_WIN32
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
                "'C:/Program Files/UniSim-1-29/Graphviz2.26.3'\n\n"
                "Alternatively, you may have to download Graphviz from www.graphviz.org.";
        throw Exception(msg);
    }
    return "\"" + command + "\"";
#else
    return "dot";
#endif
}

QString GraphGenerator::outputFilePath(OutputFormat format) {
    QDir dir = FileLocations::location(FileLocationInfo::Temporary);
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

    f.write("digraph G {size=\"16,24\";graph[rankdir=LR];\n");

    Models models = simulation->seekChildren<Model*>("*");
    for (Models::const_iterator mo = models.begin(); mo != models.end(); ++mo) {
        if (!(*mo)->hide())
            writeModel(&f, simulation, *mo);
    }
//    Ref::writeEdges(f);
	f.write("\n}\n");
}

namespace {

    void writeNode(QTextStream *sink, NamedObject *node)
	{
        QVariant label = node->property("classLabel");
        if (!label.isValid()) {
            throw Exception("Missing 'classLabel'' property", node);
        }
        QString className = label.toString();
        if (className.contains("Anonymous"))
            className.clear();
        *sink << node->uniqueId() << "[label=\"" << className;
        if (node->uniqueId() != "anonymous") {
            if (!className.isEmpty())
                *sink << "\\n";
            *sink << node->id().label();
        }
		*sink << "\"];";
	}
	
}	

void GraphGenerator::writeModel(QFile *f, NamedObject *parent, NamedObject *child)
{
    QString statement;
    QTextStream sink(&statement);

    writeNode(&sink, parent);
    writeNode(&sink, child);
    sink << parent->uniqueId() <<  "->" << child->uniqueId() <<  ";\n";

	f->write(qPrintable(statement));

	for (QList<QObject*>::const_iterator ch = child->children().begin(); ch != child->children().end(); ++ch) {
		Model *model = dynamic_cast<Model*>(*ch);
        if (model && ! model->hide())
            writeModel(f, child, model);
    }
}

