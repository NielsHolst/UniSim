/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "identifier.h"
#include "model.h"
#include "output.h"
#include "trace.h"
#include "utilities.h"
#include "variable_base.h"

namespace UniSim{
	
Trace::Trace(QString name, VariableBase *variable_, QObject *parent)
    : Component(name, parent),
      variable(variable_),
      historyCleared(false)
{
    output = dynamic_cast<Output*>(parent);
}

void Trace::amend() {
    setSummary();
    setAxis();
    setType();
}

void Trace::setSummary() {
    Summary defaultSummary =None;
    QVariant v = attribute("summary");
    if (!v.isValid()) {
        _summary = defaultSummary;
        return;
    }

    QMap<QString, Summary> summ;
    summ[""] = defaultSummary;
    summ["max"] = Max;
    summ["min"] = Min;
    summ["average"] = Average;
    summ["avg"] = Average;
    summ["sum"] = Sum;
    summ["final"] = Final;
    summ["xatmax"] = XAtMax;
    summ["xatmin"] = XAtMin;
    QString s = v.toString().toLower();
    if (summ.contains(s))
        _summary = summ[s];
    else if (s == "xatthreshold")
        throw Exception("Missing threshold value for 'XAtThreshold'."
                        "Write e.g. 'summary=\"(XAtThreshold 7.5)\"'.", this);
    else if (s.contains("xatthreshold")) {
        _summary = XAtThreshold;
        setThreshold(s);
    }
    else
        throw Exception("Unknown summary '" + v.toString() + "'", this);
}

void Trace::setThreshold(QString summaryCode) {
    QList<QString> list = decodeList<QString>(summaryCode, this);
    bool ok = (list.size() == 2) && (list[0] == "xatthreshold");
    if (ok)
        s.threshold = list[1].toDouble(&ok);
    if (!ok) {
        QString msg("Wrong value for 'summary' attribute. Use the format '(xAtThreshold 3.12)', not '%1'");
        throw Exception(msg.arg(summaryCode), this);
    }
}

void Trace::setAxis() {
    QVariant v = attribute("axis");
    if (!v.isValid())
        throw Exception("Missing 'axis' attribute", this);

    Identifier id(v.toString());
    if (id.equals("x"))
        _axis = XAxis;
    else if (id.equals("y"))
        _axis = YAxis;
    else if (id.equals("z"))
        _axis = ZAxis;
    else {
        QString msg("Axis must be either 'x', 'y' or 'z', not '%1'");
        throw Exception(msg.arg(id.label()), this);
    }
}

void Trace::setType() {
    Type defaultType = (_summary == None) ? Line : Symbols;
    QVariant v = attribute("type");
    if (!v.isValid()) {
        _type = defaultType;
        return;
    }
    QMap<QString, Type> types;
    types[""] = defaultType;
    types["line"] = Line;
    types["symbols"] = Symbols;
    types["both"] = Both;
    QString s = v.toString().toLower();
    if (!types.contains(s))
        throw Exception("Unknown type of plot: '" + v.toString() + "'", this);
    _type = types[s];
}

void Trace::reset() {
    resetSummary();
    if (!historyCleared || !isSummary())
        _history.clear();
    historyCleared = true;
}

void Trace::resetSummary() {
    s.n = 0;
    s.value = s.sum = 0.;
    switch (summary()) {
    case Max:
    case XAtMax:
        s.maxValue = -DBL_MAX;
        break;
    case Min:
    case XAtMin:
        s.minValue = DBL_MAX;
        break;
    case XAtThreshold:
        s.pastThreshold = s.hasPrevValue = false;
    default:
        ;
    }
}

void Trace::update() {
    updateSummary();
    if (!isSummary())
        _history.append(summary() == None ? currentValue() : s.value);
}

void Trace::updateSummary() {
    double value = currentValue();
    ++s.n;
    switch (summary()) {
    case None:
        s.value = value;
        break;
    case Max:
        if (value > s.maxValue)
            s.maxValue = s.value = value;
        break;
    case XAtMax:
        if (value > s.maxValue) {
            s.maxValue = value;
            s.value = s.n;
        }
        break;
    case Min:
        if (value < s.minValue)
            s.minValue = s.value = value;
        break;
    case XAtMin:
        if (value < s.minValue) {
            s.minValue = value;
            s.value = s.n;
        }
        break;
    case Average:
        s.sum += value;
        s.value = s.sum/s.n;
        break;
    case Sum:
        s.sum += value;
        s.value = s.sum;
        break;
    case Final:
        s.value = value;
        break;
    case XAtThreshold:
        if (s.pastThreshold)
            break;
        else if (!s.hasPrevValue) {
            s.prevValue = value;
            s.hasPrevValue = true;
            break;
        }
        s.pastThreshold = (s.prevValue < s.threshold && s.threshold <= value) ||
                          (s.prevValue > s.threshold && s.threshold >= value);
        s.prevValue = value;
        if (s.pastThreshold)
            s.value = s.n;
        break;
    }
}

void Trace::cleanup() {
    if (isSummary())
        _history.append(s.value);
}

void Trace::debrief() {
    historyCleared = false;
}

Trace::Axis Trace::axis() const {
    return _axis;
}

Trace::Summary Trace::summary() const {
    return _summary;
}

Trace::Type Trace::type() const {
    return _type;
}

bool Trace::hasWildCard() const {
    QString value = attribute("value").toString();
    return value.contains("*");
}

bool Trace::isSummary() const {
    return output && output->hasSummary();
}

Output* Trace::traceParent() {
    return dynamic_cast<Output*>(parent());
}

Model* Trace::variableParent() {
    return dynamic_cast<Model*>(variable->parent());
}

double Trace::currentValue() {
    QVariant v = variable->toVariant();
    return v.value<double>();
}

QVector<double>* Trace::history() {
    return &_history;
}

} //namespace

