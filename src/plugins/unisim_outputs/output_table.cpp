/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <math.h>
#include <iostream>
#include <QTextStream>
#include <usbase/file_locations.h>
#include <usbase/parameter.h>
#include <usbase/parameter_base.h>
#include <usbase/utilities.h>
#include "output_table.h"

using std::max;

namespace UniSim{
	
OutputTable::OutputTable(Identifier name, QObject *parent)
    : Output(name, parent), fileType(FileLocationInfo::Output)
{
    new Parameter<QString>("fileName", &fileName, QString("output_table.prn"), this, "description");
}


void OutputTable::cleanup() {
    if (!hasSummary()) {
        openFile();
        writeLabels();
        writeTraces();
        closeFile();
    }
}

void OutputTable::debrief() {
    if (hasSummary()) {
        openFile();
        writeLabels();
        writeTraces();
        closeFile();
    }
}

void OutputTable::openFile() {
    QString path = FileLocations::location(fileType).absolutePath();
    QString useFileName = (runNumber() == 1 || hasSummary()) ?
                          fileName :
                          amendedFileName(fileName, runNumber());
    QString filePath = path + "/" + useFileName;
    _outputFilePaths << filePath;

    file.setFileName(filePath);
    if (!file.open(QIODevice::Text | QIODevice::WriteOnly))
        throw Exception("Could not open output file to write table:\n'" + filePath + "'");

    // If more than one file then give the first file a number too
    if (runNumber() == 2 && !hasSummary()) {
        QString filePath = path + "/" + fileName;
        QFile prevFile(filePath);

        QString newName = path + "/" + amendedFileName(fileName, 1);
        _outputFilePaths[0] = newName;

        // Delete any existing file named newName
        QFile toDelete(newName);
        toDelete.remove();
        prevFile.rename(newName);
    }
}

QString OutputTable::amendedFileName(QString fileName, int number) {
    QString runCode = QString::number(number);
    runCode = runCode.rightJustified(4, '0');

    QFileInfo info(fileName);
    QString path = info.path();
    QString base = info.baseName();
    QString suffix = info.completeSuffix();
    return path + "/" + base + runCode + "." + suffix;
}

void OutputTable::writeLabels() {
    writeLabels(xTraces());
    writeTab();
    writeLabels(yTraces());
    writeCR();
}

void OutputTable::writeLabels(const QList<TraceRecord> &traces) {
    if (traces.isEmpty())
        return;
    QString s;
    QTextStream text(&s);
    text << traces[0].label;
    for (int i = 1; i < traces.size(); ++i)
        text << "\t" << traces[i].label;
    file.write(qPrintable(s));
}

void OutputTable::writeTraces() {
    int n = traceSize();
    for (int i = 0; i < n; ++i) {
        writeTraces(xTraces(), i);
        writeTab();
        writeTraces(yTraces(), i);
        writeCR();
    }
}

int OutputTable::traceSize() const {
    if (traces().isEmpty())
        return 0;
    int theSize = -1;
    for (int i = 0; i < traces().size(); ++i) {
        int nextSize = traces()[i]->history()->size();
        if (theSize == -1)
            theSize = nextSize;
        else if (nextSize != theSize) {
            QString msg ("Output variable data buffers are of unequal size. %1 vs. %2");
            throw Exception(msg.arg(nextSize).arg(theSize));
        }
    }
    return theSize;
}

void OutputTable::writeTraces(const QList<TraceRecord> &traces, int dataIx) {
    if (traces.isEmpty())
        return;
    QString s;
    QTextStream text(&s);
    text.setRealNumberPrecision(10);
    text << traces.at(0).trace->history()->value(dataIx);
    for (int i = 1; i < traces.size(); ++i) {
        text << "\t" << traces.at(i).trace->history()->value(dataIx);
    }
    file.write(qPrintable(s));
}

void OutputTable::closeFile() {
    file.close();
}

void OutputTable::writeTab() {
    file.write("\t");
}

void OutputTable::writeCR() {
    file.write("\n");
}

QStringList OutputTable::outputFilePaths() {
    return _outputFilePaths;
}

} //namespace

