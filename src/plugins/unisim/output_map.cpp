/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QTextStream>
#include <usbase/exception.h>
#include <usbase/model.h>
#include <usbase/trace.h>
#include "output_map.h"

using std::max;

namespace UniSim{
	
OutputMap::OutputMap(Identifier name, QObject *parent)
    : OutputTableBase(name, parent)
{
}

void OutputMap::initialize() {
    if (hasSummary())
        throw Exception("Output maps cannot have summary traces", this);
    traces.clear();
    positions.clear();
    int n = traceRecords().size();
    for (int i = 0; i < n; ++i) {
        Trace *trace = traceRecords().at(i).trace;
        traces << trace;
        int x = trace->variableParent()->pullValue<int>("x");
        int y = trace->variableParent()->pullValue<int>("y");
        positions << qMakePair(x,y);
    }
}

void OutputMap::cleanup() {
    openFile();
    writeTraces();
    closeFile();
}

void OutputMap::writeTraces() {
    writeString("<svg width=\"100%\" height=\"100%\" xmlns=\"http://www.w3.org/2000/svg\">\n");

    int T = traces.first()->history()->size();
    int n = traces.size();
    for (int t = 0; t < T; ++t) {
        writeTime(t, t==T-1);
        writeString("<g visibility=\"hidden\">\n");
        for (int i = 0; i < n; ++i) {
            const Position &pos(positions.at(i));
            Trace *trace = traces[i];
            writeRect(pos, trace->history()->at(t));
        }
        writeRectGroupEnd(t);
    }
    writeString("</svg>\n");
}

void OutputMap::writeTime(int time, bool atEnd) {
    QString s =
        "<g visibility=\"hidden\">\n"
        "    <text x=\"10\" y=\"20\" font-size=\"16\" >Time =  %1</text>\n"
        "    %2"
        "</g>\n\n";
    writeString(s.arg(time).arg(buildSet(time, "a", atEnd)));
}

QString OutputMap::buildSet(int time, QString subId, bool atEnd) {
    QString end = "g%1a.end";
    QString begin = (time == 0 ) ? "0s" : end.arg(time-1);
    QString duration = (subId == "a" && !atEnd) ? "dur=\"0.1s\"" : "fill=\"freeze\"";
    QString s =
        "<set id=\"g%1%2\" attributeName=\"visibility\" attributeType=\"CSS\" to=\"visible\" begin=\"%3\" %4/>\n";
    return s.arg(time).arg(subId).arg(begin).arg(duration);
}

void OutputMap::writeRect(Position pos, double value) {
    QString s = "    <rect x=\"%1\" y=\"%2\" width=\"40\" height=\"40\" style=\"fill:yellow %3\"/>\n";
    writeString(s.arg(pos.first).arg(pos.second).arg(value));
}

void OutputMap::writeRectGroupEnd(int time) {
    writeString("    " + buildSet(time, "b"));
    writeString("</g>\n\n");
}

} //namespace

