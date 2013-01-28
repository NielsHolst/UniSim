/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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
#include "output_table_base.h"
#include <usbase/trace.h>

using std::max;

namespace UniSim{
	
OutputTableBase::OutputTableBase(Identifier name, QObject *parent)
    : Output(name, parent)
{
    new Parameter<QString>("fileName", &fileName, QString("output.txt"), this, "Name of output file");
}

int OutputTableBase::numXTraces() const {
    return 0;
}

void OutputTableBase::openFile(QString appendix) {
    QString fileNameApp = insertAppendix(fileName, appendix);
    QString useFileName = (runNumber() == 1 || hasSummary()) ?
                          fileNameApp : insertNumber(fileNameApp, runNumber());

    QString path = FileLocations::location(FileLocationInfo::Output).absolutePath();
    QString filePath = path + "/" + useFileName;
    outputFilePaths << filePath;

    file.setFileName(filePath);
    if (!file.open(QIODevice::Text | QIODevice::WriteOnly))
        throw Exception("Could not open output file to write table:\n'" + filePath + "'");

    // If more than one file then give the first file a number too
    if (runNumber() == 2 && !hasSummary()) {
        QString filePath = path + "/" + fileNameApp;
        QFile prevFile(filePath);

        QString newName = path + "/" + insertNumber(fileNameApp, 1);
        outputFilePaths[0] = newName;

        // Delete any existing file named newName
        QFile toDelete(newName);
        toDelete.remove();
        prevFile.rename(newName);
    }
}

QString OutputTableBase::insertAppendix(QString fileName, QString appendix) {
    QFileInfo info(fileName);
    QString path = info.path();
    QString base = info.baseName();
    QString suffix = info.completeSuffix();
    QString app = appendix.isEmpty() ? "" : ("_" + appendix);
    return path + "/" + base + app + "." + suffix;
}

QString OutputTableBase::insertNumber(QString fileName, int number) {
    QString runCode = QString::number(number);
    runCode = runCode.rightJustified(4, '0');
    return insertAppendix(fileName, runCode);
}

void OutputTableBase::writeString(QString s) {
    file.write(qPrintable(s));
}

void OutputTableBase::closeFile() {
    file.close();
}

int OutputTableBase::traceSize() {
    if (traceRecords().isEmpty())
        return 0;
    int theSize = -1;
    for (int i = 0; i < traceRecords().size(); ++i) {
        int nextSize = traceRecords().at(i).trace->history()->size();
        if (theSize == -1)
            theSize = nextSize;
        else if (nextSize != theSize) {
            QString msg ("Output variable data buffers are of unequal size. %1 vs. %2");
            throw Exception(msg.arg(nextSize).arg(theSize));
        }
    }
    return theSize;
}

} //namespace

