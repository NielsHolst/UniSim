/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QTextStream>
#include <usbase/file_locations.h>
#include <usbase/output_variable.h>
#include <usbase/parameter.h>
#include <usbase/parameter_base.h>
#include <usbase/utilities.h>
#include "output_parameter_table.h"

namespace UniSim{
	
OutputParameterTable::OutputParameterTable(Identifier name, QObject *parent)
    : Output(name, parent)
{
    new Parameter<QString>("fileName", &fileName, QString("output_table.prn"), this, "description");
}

void OutputParameterTable::cleanup() {
    openFile();
    writeLabels(xVariables());
    writeTab();
    writeLabels(yVariables());
    writeCR();

    int dataSize = checkDataSize(xVariables());
    checkDataSize(yVariables(), dataSize);

    for (int i = 0; i < dataSize; ++i) {
        writeData(xVariables(), i);
        writeTab();
        writeData(yVariables(), i);
        writeCR();
    }
    file.close();
}

void OutputParameterTable::openFile() {
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

QString OutputParameterTable::ammendedFileName(QString fileName, int number) {
    QString runCode = QString::number(number);
    runCode = runCode.rightJustified(4, '0');

    QFileInfo info(fileName);
    QString path = info.path();
    QString base = info.baseName();
    QString suffix = info.completeSuffix();
    return path + "/" + base + runCode + "." + suffix;
}

void OutputParameterTable::writeLabels(const OutputVariables &variables) {
    if (variables.isEmpty())
        return;
    QString s;
    QTextStream text(&s);
    text << variables[0]->id().label();
    for (int i = 1; i < variables.size(); ++i)
        text << "\t" << variables[i]->id().label();
    file.write(qPrintable(s));
}

void OutputParameterTable::writeTab() {
    file.write("\t");
}

void OutputParameterTable::writeCR() {
    file.write("\n");
}

int OutputParameterTable::checkDataSize(const OutputVariables &variables, int dataSize) const {
    for (int i = 0; i < variables.size(); ++i) {
        int thisSize = variables[i]->history()->size();
        if (dataSize > 0 && dataSize != thisSize)
            throw Exception("Output variable data buffers of unequal size");
        else
            dataSize = thisSize;
    }
    return dataSize;
}

void OutputParameterTable::writeData(const OutputVariables &variables, int dataIx) {
    if (variables.isEmpty())
        return;
    QString s;
    QTextStream text(&s);
    text << variables[0]->history()->value(dataIx);
    for (int i = 1; i < variables.size(); ++i)
        text << "\t" << variables[i]->history()->value(dataIx);
    file.write(qPrintable(s));
}

void OutputParameterTable::writeParameters() {
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

