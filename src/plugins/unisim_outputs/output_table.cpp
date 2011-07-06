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
    writeXLabels();
    writeTab();
    setYLabels();
    writeYLabels();
    writeCR();
}

void OutputTable::writeResults() {
    int n = resultsSize();
    for (int i = 0; i < n; ++i) {
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

void OutputTable::writeXLabels() {
    if (xResults().isEmpty())
        return;
    QString s;
    QTextStream text(&s);
    text << xResults()[0]->id().label();
    for (int i = 1; i < xResults().size(); ++i)
        text << "\t" << xResults()[i]->id().label();
    file.write(qPrintable(s));
}

void OutputTable::writeYLabels() {
    if (yLabels.isEmpty())
        return;
    QString s;
    QTextStream text(&s);
    text << yLabels[0].label();
    for (int i = 1; i < yLabels.size(); ++i)
        text << "\t" << yLabels[i].label();
    file.write(qPrintable(s));
}

void OutputTable::writeTab() {
    file.write("\t");
}

void OutputTable::writeCR() {
    file.write("\n");
}

int OutputTable::resultsSize() const {
    int xSize = dataSize(xResults());
    int ySize = dataSize(yResults());
    bool ok = xSize==0 || ySize==0 || xSize==ySize;
    if (!ok)
        throw Exception("x and y output variable data buffers are of unequal size");
    return max(xSize, ySize);
}

int OutputTable::dataSize(const OutputResults &results) const {
    if (results.isEmpty()) return 0;
    int theSize = -1;
    for (int i = 0; i < results.size(); ++i) {
        int nextSize = results[i]->history()->size();
        if (theSize == -1)
            theSize = nextSize;
        else if (nextSize != theSize)
            throw Exception("Output variable data buffers are of unequal size");
    }
    return theSize;
}

void OutputTable::writeResults(const OutputResults &results, int dataIx) {
    if (results.isEmpty())
        return;

    QString s;
    QTextStream text(&s);
    text.setRealNumberPrecision(10);
    text << results[0]->history()->value(dataIx);
    for (int i = 1; i < results.size(); ++i)
        text << "\t" << results[i]->history()->value(dataIx);
    file.write(qPrintable(s));
}

} //namespace

