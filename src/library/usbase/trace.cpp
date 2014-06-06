/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <limits>
#include <cmath>
#include "exception.h"
#include "identifier.h"
#include "model.h"
#include "output.h"
#include "trace.h"
#include "decode_list.h"
#include "ustime.h"
#include "variable_base.h"

using std::numeric_limits;
using std::floor;

namespace UniSim{
	
Trace::Trace(QString name, VariableBase *variable_, QObject *parent)
    : Component(name, parent),
      variable(variable_),
      multiplier(1.), divisor(1.),
      historyCleared(false)
{
    InputRef(int, sample, "..[sample]");
    output = dynamic_cast<OutputBase*>(parent);
}

void Trace::amend() {
    setScaling();
    setSummary();
    setType();
}

void Trace::setScaling() {
    if (hasAttribute("multiplier")) {
        multiplier = attribute("multiplier").value<double>();
        if (multiplier == 0.)
            throw Exception("Trace multiplier cannot be zero", this);
    }
    if (hasAttribute("divisor")) {
        if (divisor == 0.)
            throw Exception("Trace divisor cannot be zero", this);
        divisor = attribute("divisor").value<double>();
    }
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

void Trace::setType() {
    Type defaultType = (_summary == None) ? Line : Symbols;
    QVariant v = attribute("type");
    if (!v.isValid() || v.toString().toLower()=="time") {
        _type = defaultType;
        setAttribute("label", "test");
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
    sampleCount = 0;
    sampleSum = 0.;
}

void Trace::resetSummary() {
    s.n = 0;
    s.value = s.sum = 0.;
    switch (summary()) {
    case Max:
    case XAtMax:
        s.maxValue = -numeric_limits<double>::max();
        break;
    case Min:
    case XAtMin:
        s.minValue = numeric_limits<double>::max();
        break;
    case XAtThreshold:
        s.pastThreshold = s.hasPrevValue = false;
    default:
        ;
    }
}

inline Time::Unit suggestedUnit(long secs) {
    if (secs <= Time(5, Time::Minutes).toSeconds())
        return Time::Seconds;
    else if (secs <= Time(5, Time::Hours).toSeconds())
        return Time::Minutes;
    else if (secs <= Time(48, Time::Hours).toSeconds())
        return Time::Hours;
    else if (secs <= Time(500, Time::Days).toSeconds())
        return Time::Days;
    return Time::Years;
}

void Trace::initialize() {
    QVariant v = attribute("type");
    bool isTime =  v.isValid() && v.toString().toLower()=="time";
    if (isTime) {
        Model *steps = seekOne<Model*>("steps");
        Model *calendar = seekOne<Model*>("calendar");
        int maxSteps = steps->pullValue<int>("maxSteps");
        int timeStepSecs = floor(calendar->pullValue<double>("timeStepSecs") + 0.5);
        long duration = long(maxSteps)*timeStepSecs;
//        int timeChar = calendar->pullValue<char>("timeUnit");
//        Time::Unit unit = Time::charToUnit(timeChar),
//                   newUnit = suggestedUnit(duration);
//        int timeChar = calendar->pullValue<char>("timeUnit");
        Time::Unit unit = Time::Seconds,
                   newUnit = suggestedUnit(duration);
        divisor = (newUnit > unit)
                  ? Time(1,newUnit).toSeconds() / Time(1,unit).toSeconds() // / timeStepSecs
                  : 1;
        multiplier = timeStepSecs;
    }
}

void Trace::update() {
    updateSummary();
    if (!isSummary()) {
        sampleSum += summary() == None ? currentValue() : s.value;
        if (sampleCount++ == sample) {
            _history.append(sampleSum/sampleCount);
            sampleSum = 0.;
            sampleCount = 0;
        }
    }
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

OutputBase* Trace::traceParent() {
    return dynamic_cast<OutputBase*>(parent());
}

Model* Trace::variableParent() {
    auto model = dynamic_cast<const Model*>(variable->parent());
    return const_cast<Model*>(model);
}

double Trace::currentValue() {
    QVariant v = variable->toVariant();
    return v.value<double>()*multiplier/divisor;
}

QVector<double>* Trace::history() {
    return &_history;
}

} //namespace

