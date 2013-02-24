/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <cmath>
#include <QTextStream>
#include <usbase/exception.h>
#include <usbase/model.h>
#include <usbase/trace.h>
#include <usbase/utilities.h>
#include "output_map.h"

using std::max;

namespace UniSim{
	
OutputMap::OutputMap(Identifier name, QObject *parent)
    : OutputTableBase(name, parent)
{
    new Parameter<QString>("range", &rangeAsString, "(0 100)", this,
        "Range of trace values on colour ramp. Use log-transformed range values if @F log10 is set to @F {true}");
    new Parameter<int>("steps", &steps, 7, this,
        "Number of steps on colour ramp");
    new Parameter<bool>("log10", &doLog10, false, this,
        "Log-transform trace values?");
}

void OutputMap::initialize() {
    if (hasSummary())
        throw Exception("Output maps cannot have summary traces", this);
    traces.clear();
    setRange();
    setPatches();
}

void OutputMap::setRange() {
    QList<double> v = decodeList<double>(rangeAsString, this);
    if (v.size() != 2) {
        QString msg = "List must hold two numbers for a range: '%1'";
        throw Exception(msg.arg(rangeAsString));
    }
    range.first = v.at(0);
    range.second = v.at(1);
    if (range.first >= range.second) {
        QString msg = "Numbers in range must be in increasing order: '%1'";
        throw Exception(msg.arg(rangeAsString));
    }
}

void OutputMap::setPatches() {
    patches.clear();
    int n = traceRecords().size();
    for (int i = 0; i < n; ++i) {
        Trace *trace = traceRecords().at(i).trace;
        traces << trace;
        PatchInfo pi;
        pi.x = findValue(trace, "x");
        pi.y = findValue(trace, "y");
        pi.size = findValue(trace, "size");
        patches << pi;
    }
}


int OutputMap::findValue(Trace *trace, QString name) {
    Model *model = trace->variableParent();
    Parameter<int> *parameter = model->seekOneNearest<Parameter<int>*>(name);
    return parameter->value();
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
            const PatchInfo &pi(patches.at(i));
            Trace *trace = traces[i];
            writeRect(pi, trace->history()->at(t));
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

void OutputMap::writeRect(PatchInfo pi, double value) {
    QString s = "    <rect x=\"%1\" y=\"%2\" width=\"%3\" height=\"%3\" style=\"fill:%4\"/>\n";
    writeString(s.arg(pi.x).arg(pi.y+40).arg(pi.size).arg(colour(value)));
}

QString OutputMap::colour(double value) {
    double x;
    if (doLog10) {
        if (value <= 0.)
            return "#000000";
        x = log10(value);
    }
    else {
        x = value;
    }
    double prop = (x - range.first)/(range.second - range.first);
    int red;
    if (prop < 0)
        red = 0;
    else if (prop > 1)
        red = 255;
    else
        red = int (prop*255);
    return "#" + QString::number(red,16) + "FFFF";
}

void OutputMap::writeRectGroupEnd(int time) {
    writeString("    " + buildSet(time, "b"));
    writeString("</g>\n\n");
}

} //namespace

