/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QTextStream>
#include <usbase/file_locations.h>
#include <usbase/parameter.h>
#include <usbase/parameter_base.h>
#include <usbase/utilities.h>
#include "output_table.h"

namespace UniSim{
	
OutputTable::OutputTable(Identifier name, QObject *parent)
    : Output(name, parent)
{
    new Parameter<QString>("fileName", &fileName, QString("output_table.prn"), this, "description");
}


void OutputTable::cleanup() {
    if (!isSummary()) {
        openFile();
        writeLabels();
        writeResults();
        closeFile();
    }
}

void OutputTable::debrief() {
    if (isSummary()) {
        openFile();
        writeLabels();
        writeResults();
        closeFile();
    }
}

void OutputTable::openFile() {
    QString path = FileLocations::location(FileLocationInfo::Output).absolutePath();
    QString useFileName = (runNumber() == 1 || isSummary()) ?
                          fileName :
                          ammendedFileName(fileName, runNumber());
    QString filePath = path + "/" + useFileName;

    file.setFileName(filePath);
    if (!file.open(QIODevice::Text | QIODevice::WriteOnly))
        throw Exception("Could not open output file to write table:\n'" + filePath + "'");

    // If more than one file then give the first file a number too
    if (runNumber() == 2 && !isSummary()) {
        QString filePath = path + "/" + fileName;
        QFile prevFile(filePath);

        QString newName = path + "/" + ammendedFileName(fileName, 1);

        // Delete any existing file named newName
        QFile toDelete(newName);
        toDelete.remove();
        prevFile.rename(newName);
    }
}

void OutputTable::writeLabels() {
    writeLabels(xResults());
    writeTab();
    writeLabels(yResults());
    writeCR();
}

void OutputTable::writeResults() {
    int dataSize = checkDataSize(xResults());
    checkDataSize(yResults(), dataSize);

    for (int i = 0; i < dataSize; ++i) {
        writeResults(xResults(), i);
        writeTab();
        writeResults(yResults(), i);
        writeCR();
    }
}

void OutputTable::closeFile() {
    file.close();
}

QString OutputTable::ammendedFileName(QString fileName, int number) {
    QString runCode = QString::number(number);
    runCode = runCode.rightJustified(4, '0');

    QFileInfo info(fileName);
    QString path = info.path();
    QString base = info.baseName();
    QString suffix = info.completeSuffix();
    return path + "/" + base + runCode + "." + suffix;
}

void OutputTable::writeLabels(const OutputResults &results) {
    if (results.isEmpty())
        return;
    QString s;
    QTextStream text(&s);
    text << results[0]->id().label();
    for (int i = 1; i < results.size(); ++i)
        text << "\t" << results[i]->id().label();
    file.write(qPrintable(s));
}

void OutputTable::writeTab() {
    file.write("\t");
}

void OutputTable::writeCR() {
    file.write("\n");
}

int OutputTable::checkDataSize(const OutputResults &results, int dataSize) const {
    for (int i = 0; i < results.size(); ++i) {
        int thisSize = results[i]->history()->size();
        if (dataSize > 0 && dataSize != thisSize)
            throw Exception("Output variable data buffers of unequal size");
        else
            dataSize = thisSize;
    }
    return dataSize;
}

void OutputTable::writeResults(const OutputResults &results, int dataIx) {
    if (results.isEmpty())
        return;
    QString s;
    QTextStream text(&s);
    text << results[0]->history()->value(dataIx);
    for (int i = 1; i < results.size(); ++i)
        text << "\t" << results[i]->history()->value(dataIx);
    file.write(qPrintable(s));
}

} //namespace

