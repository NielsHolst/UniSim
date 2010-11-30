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
    openFile();
    writeLabels(xResults());
    writeTab();
    writeLabels(yResults());
    writeCR();

    int dataSize = checkDataSize(xResults());
    checkDataSize(yResults(), dataSize);

    for (int i = 0; i < dataSize; ++i) {
        writeData(xResults(), i);
        writeTab();
        writeData(yResults(), i);
        writeCR();
    }
    file.close();
}

void OutputTable::openFile() {
    QString path = FileLocations::location(FileLocationInfo::Output).absolutePath();
    QString useFileName = runNumber() == 1 ?
                          fileName :
                          ammendedFileName(fileName, runNumber());
    QString filePath = path + "/" + useFileName;

    file.setFileName(filePath);
    if (!file.open(QIODevice::Text | QIODevice::WriteOnly))
        throw Exception("Could not open output file to write table:\n'" + filePath + "'");

    // If more than one file then give the first file a number too
    if (runNumber() == 2) {
        writeParameters();

        QString filePath = path + "/" + fileName;
        QFile prevFile(filePath);

        QString newName = path + "/" + ammendedFileName(fileName, 1);
        prevFile.rename(newName);
    }
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

void OutputTable::writeData(const OutputResults &results, int dataIx) {
    if (results.isEmpty())
        return;
    QString s;
    QTextStream text(&s);
    text << results[0]->history()->value(dataIx);
    for (int i = 1; i < results.size(); ++i)
        text << "\t" << results[i]->history()->value(dataIx);
    file.write(qPrintable(s));
}

void OutputTable::writeParameters() {
    QString path = FileLocations::location(FileLocationInfo::Output).absolutePath();
    QString filePath = path + "/" + "param.txt";
    QFile file(filePath);
    if (!file.open(QIODevice::Text | QIODevice::WriteOnly))
        throw Exception("Could not open output file to write parameters:\n'" + filePath + "'");

    QList<ParameterBase*> param = seekMany<ParameterBase*>("*");
    for (int i = 0; i < param.size(); ++i) {
        QString s;
        QTextStream text(&s);
        text << UniSim::fullName(param[i]) << "\t" << param[i]->toVariant().toString() << "\n";
        file.write(qPrintable(s));
    }
}

} //namespace

