/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "identifier.h"
#include "output.h"
#include "output_result.h"

namespace UniSim{
	
OutputResult::OutputResult(QString label, QString axis, QObject *parent)
    : Component(label, parent), _summary(None), historyCleared(false)
{
    output = dynamic_cast<Output*>(parent);
    setAxisFromString(axis);
}

void OutputResult::setAxisFromString(QString axis) {
    Identifier id(axis);
    if (id.equals("x"))
        _axis = XAxis;
    else if (id.equals("y"))
        _axis = YAxis;
    else
        throw Exception("Axis must be either 'x'' or 'y', not '" + axis + "'", this);
}

void OutputResult::setSummaryFromString(QString summary) {
    Identifier id(summary);
    if (id.equals(""))
        _summary = None;
    else if (id.equals("max"))
        _summary = Max;
    else if (id.equals("min"))
        _summary = Min;
    else if (id.equals("average") || id.equals("avg"))
        _summary = Average;
    else if (id.equals("final"))
        _summary = Final;
    else if (id.equals("xatthreshold"))
        throw Exception("Missing threshold value for 'XAtThreshold'."
                        "Write e.g. 'summary=\"(XAtThreshold 7.5)\"'.", this);
    else if (id.key().contains("xatthreshold"))
        _summary = XAtThreshold;
    else if (id.equals("xatmax"))
        _summary = XAtMax;
    else if (id.equals("xatmin"))
        _summary = XAtMin;
    else
        throw Exception("Unknown summary '" + summary + "'", this);
}

void OutputResult::reset() {
    if (!historyCleared || !isOutputSummary())
        _history.clear();
    historyCleared = true;
}

void OutputResult::debrief() {
    historyCleared = false;
}


OutputResult::Axis OutputResult::axis() const {
    return _axis;
}

OutputResult::Summary OutputResult::summary() const {
    return _summary;
}

const QVector<double>* OutputResult::history() const {
    return &_history;
}

bool OutputResult::isOutputSummary() const {
    return output && output->isSummary();
}

} //namespace

