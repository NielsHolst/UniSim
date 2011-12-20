/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <cfloat>
#include <QMessageBox>
#include <QPair>
#include "exception"
#include "identifier.h"
#include "output.h"
#include "model.h"
#include "output_variable.h"
#include "pull_variable.h"
#include "utilities.h"

namespace UniSim{
	
OutputVariable::OutputVariable(const QMap<QString,QString> &attributes,
                               PullVariableBase *variable,
                               QObject *parent)
    : OutputResult(attributes, parent),
      summaryString(attributes.value("summary")),
      pullVarPtr(variable)
{
    Q_ASSERT(pullVarPtr);
}

void OutputVariable::initialize() {
    OutputResult::initialize();
    setSummaryFromString(summaryString);
    if (summary() == XAtThreshold){
        setThreshold(summaryString);
    }
}

void OutputVariable::setThreshold(QString summary) {
    QPair<QString, double> decoded = decodeSummary(summary);
    QString name = decoded.first;
    s.threshold = decoded.second;
    if (!Identifier(name).equals("XAtThreshold"))
        throw Exception("Summary type 'XAtThreshold' expected", this);
}

QPair<QString, double> OutputVariable::decodeSummary(QString summary) {
    QString s = summary.simplified();
    if (s.left(1)!="(" ||  s.right(1)!=")")
        throw Exception("Write summary as (name value) pair", this);
    s = s.mid(1, s.length()-2);
    QStringList parts = s.split(" ");
    if (parts.size() != 2)
        throw Exception("Write summary as (name value) pair", this);
    QPair<QString, double> result;
    bool ok;
    result.first = parts[0];
    result.second = parts[1].toDouble(&ok);
    if (!ok)
        throw Exception("Write summary as (name value) pair. Error in value", this);
    return result;
}


void OutputVariable::reset() {
    OutputResult::reset();
    resetSummary();
}

void OutputVariable::update() {
    Q_ASSERT(pullVarPtr);
    double value = pullVarPtr->toVariant().value<double>();
    updateSummary(value);
    if (!isOutputSummary())
        _history.append(summary() == None ? value : s.value);
}

void OutputVariable::cleanup() {
    if (isOutputSummary())
        _history.append(s.value);
}

void OutputVariable::resetSummary() {
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
        s.passedThreshold = s.hasPrevValue = false;
    default:
        ;
    }
}

void OutputVariable::updateSummary(double value) {
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
        if (s.passedThreshold)
            break;
        else if (!s.hasPrevValue) {
            s.prevValue = value;
            s.hasPrevValue = true;
            break;
        }
        s.passedThreshold = (s.prevValue < s.threshold && s.threshold <= value) ||
                          (s.prevValue > s.threshold && s.threshold >= value);
        s.prevValue = value;
        if (s.passedThreshold)
            s.value = s.n;
        break;
    }
}

const PullVariableBase* OutputVariable::pullVariable() {
    return pullVarPtr;
}

} //namespace

