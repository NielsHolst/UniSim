/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <qfileinfo.h>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/trace.h>
#include <usbase/utilities.h>
#include "output_crosstab.h"

using std::max;

namespace UniSim{
	
OutputCrosstab::OutputCrosstab(Identifier name, QObject *parent)
    : OutputTableBase(name, parent)
{
}

void OutputCrosstab::amend() {
    OutputTableBase::amend();
    checkTraces();
    for (int i = 0; i < yTraces().size(); ++i) {
        Trace *trace = yTraces()[i].trace;
        Model *rowParent = seekParent(trace, rowClass);
        Model *columnParent = seekParent(trace, columnClass);
        QString rowName = rowParent->id().label();
        QString columnName = columnParent->id().label();
        TraceKey key = qMakePair(rowName, columnName);
        if (traceMatrix.contains(key)) {
            QString msg("Crosstab entry is not unique. More than one entry for row '%1' and column '%2");
            throw Exception(msg.arg(rowName).arg(columnName));
        }
        if (!rowNames.contains(rowName))
            rowNames << rowName;
        if (!columnNames.contains(columnName))
            columnNames << columnName;
        traceMatrix[ key ] = trace;
    }
}

void OutputCrosstab::checkTraces() {
    if (yTraces().isEmpty()) {
        QString msg("Crosstab output must have at least one 'y' variable");
        throw Exception(msg, this);
    }
    Trace *y = yTraces()[0].trace;
    checkAttribute(y, "rows");
    checkAttribute(y, "columns");
    rowClass = y->attribute("rows").toString();
    columnClass = y->attribute("columns").toString();

    for (int i = 1; i < yTraces().size(); ++i) {
        Trace *y = yTraces()[i].trace;
        checkAttribute(y, "rows");
        checkAttribute(y, "columns");
        checkAttribute(y, "rows", rowClass);
        checkAttribute(y, "columns", columnClass);
    }
}

void OutputCrosstab::checkAttribute(Trace *trace, QString attr) {
    if (!trace->hasAttribute(attr) || trace->attribute(attr).toString().isEmpty()) {
        QString msg("Variable on 'y' axis must have a '%1' attribute in Crosstab output");
        throw Exception(msg.arg(attr), trace);
    }
}

void OutputCrosstab::checkAttribute(Trace *trace, QString attr, QString value) {
    QString value2 = trace->attribute(attr).toString();
    if (value2 != value) {
        QString msg("Variables on 'y' axis must have matching values for '%1' attribute in Crosstab output."
                    "These do not match: '%2' and '%3'");
        throw Exception(msg.arg(attr).arg(value2).arg(value), trace);
    }
}

Model* OutputCrosstab::seekParent(Trace *trace, QString parentClass) {
    Model *parent = trace->variableParent();
    while (parent) {
        if (parent->classId().equals(parentClass))
            return parent;
        parent = parent->seekParent<Model*>("*");
    }
    QString msg("No parent of class '%1' found for trace '%2'");
    throw Exception(msg.arg(parentClass.arg(trace->id().label())));
}

void OutputCrosstab::cleanup() {
    if (!hasSummary())
        writeFiles();
}

void OutputCrosstab::debrief() {
    if (hasSummary())
        writeFiles();
}

void OutputCrosstab::writeFiles() {
    int numFiles = traceSize();
    for (int t = 0; t < numFiles; ++t) {
        QString fileNumber = QString::number(t+1).rightJustified(4, '0');
        openFile(fileNumber);
        writeColumnLabels();
        writeHistoryToFile(t);
        closeFile();
    }
}

void OutputCrosstab::writeColumnLabels() {
    writeString(rowClass + "\t");
    writeString(columnNames.join("\t"));
    writeString("\n");
}

void OutputCrosstab::writeHistoryToFile(int ixHistory) {
    int nRows = rowNames.size();
    int nColumns = columnNames.size();
    for (int i = 0; i < nRows; ++i) {
        QString rowName = rowNames.at(i);
        writeString(rowName);
        for (int j = 0; j < nColumns; ++j) {
            QString columnName = columnNames.at(j);
            TraceKey key = qMakePair(rowName, columnName);
            QString value;
            if (traceMatrix.contains(key)) {
                Trace *trace = traceMatrix.value(key);
                value = QString::number(trace->history()->at(ixHistory));
            }
            else
                value = "NA";
            writeString("\t" + value);
        }
        writeString("\n");
    }
}

} //namespace

