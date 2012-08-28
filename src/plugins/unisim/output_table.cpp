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
#include <usbase/trace.h>
#include <usbase/utilities.h>
#include "output_table.h"

using std::max;

namespace UniSim{
	
OutputTable::OutputTable(Identifier name, QObject *parent)
    : OutputTableBase(name, parent)
{
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

void OutputTable::writeLabels() {
    writeLabels(xTraces());
    writeString("\t");
    writeLabels(yTraces());
    writeString("\n");
}

void OutputTable::writeLabels(const QList<TraceRecord> &traces) {
    if (traces.isEmpty())
        return;
    QString s;
    QTextStream text(&s);
    text << traces[0].label;
    for (int i = 1; i < traces.size(); ++i)
        text << "\t" << traces[i].label;
    writeString(s);
}

void OutputTable::writeTraces() {
    int n = traceSize();
    for (int i = 0; i < n; ++i) {
        writeTraces(xTraces(), i);
        writeString("\t");
        writeTraces(yTraces(), i);
        writeString("\n");
    }
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
    writeString(s);
}

//QStringList OutputTable::outputFilePaths() {
//    return _outputFilePaths;
//}

} //namespace

