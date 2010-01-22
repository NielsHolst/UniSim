/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/file_locations.h>
#include <usbase/output_variable.h>
#include "output_table.h"

namespace UniSim{
	
OutputTable::OutputTable(Identifier name, QObject *parent)
    : Output(name, parent)
{
}

void OutputTable::initialize() {
    Output::initialize();

    for (int  i = 0; i < xVariables().size(); ++i)
        xVariables()[i]->standardizeLabel();
    for (int  i = 0; i < yVariables().size(); ++i)
        yVariables()[i]->standardizeLabel();

    setParameter("fileName", &fileName, QString("output_table.prn"));
}

void OutputTable::cleanup() {
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

void OutputTable::openFile() {
    QString path = FileLocations::location(FileLocations::Output).absolutePath();
    QString filePath = path + "/" + fileName;
    file.setFileName(filePath);
    if (!file.open(QIODevice::Text | QIODevice::WriteOnly))
        throw Exception("Could not open output file to write table:\n'" + filePath + "'");
}

void OutputTable::writeLabels(const OutputVariables &variables) {
    QString s;
    QTextStream text(&s);
    text << variables[0]->label();
    for (int i = 1; i < variables.size(); ++i)
        text << "\t" << variables[i]->label();
    file.write(qPrintable(s));
}

void OutputTable::writeTab() {
    file.write("\t");
}

void OutputTable::writeCR() {
    file.write("\n");
}

int OutputTable::checkDataSize(const OutputVariables &variables, int dataSize) const {
    for (int i = 0; i < variables.size(); ++i) {
        int thisSize = variables[i]->data()->size();
        if (dataSize > 0 && dataSize != thisSize)
            throw Exception("Output variable data buffers of unequal size");
        else
            dataSize = thisSize;
    }
    return dataSize;
}

void OutputTable::writeData(const OutputVariables &variables, int dataIx) {
    QString s;
    QTextStream text(&s);
    text << variables[0]->data()->value(dataIx);
    for (int i = 1; i < variables.size(); ++i)
        text << "\t" << variables[i]->data()->value(dataIx);
    file.write(qPrintable(s));
}


} //namespace

