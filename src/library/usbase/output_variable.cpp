/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include "model.h"
#include "output_variable.h"
#include "pull_variable.h"
#include "utilities.h"

namespace UniSim{
	
OutputVariable::OutputVariable(QString label, QString axis, PullVariableBase *variable, QObject *parent)
    : Component(label, parent), pullVarPtr(variable)
{
    Q_ASSERT(pullVarPtr);
    setAxisFromString(axis);
}

void OutputVariable::setAxisFromString(QString axis) {
    QString s = axis.toLower();
    if (axis!="x" && axis!="y")
        throw Exception("Axis must be either 'x'' or 'y', not '" + axis + "'");
    _axis = axis == "x" ? XAxis : YAxis;
}

void OutputVariable::reset() {
    _history.clear();
}

void OutputVariable::update() {
    Q_ASSERT(pullVarPtr);
    double toAppend = pullVarPtr->toVariant().value<double>();
    _history.append(toAppend);
}

OutputVariable::Axis OutputVariable::axis() const {
    return _axis;
}

const QVector<double>* OutputVariable::history() const {
    return &_history;
}

} //namespace

