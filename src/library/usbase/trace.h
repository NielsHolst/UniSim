/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_TRACE_H
#define UNISIM_TRACE_H

#include <QList>
#include <QVector>
#include "component.h"
#include "model.h"
#include "trace_base.h"

namespace UniSim{	

template <class T>
class Trace : public TraceBase
{
public:
    Trace(QString name, T *variable, QObject *parent = 0);
    Output* traceParent();
    Model* variableParent();
    double currentValue();
private:
    const T *variable;
};

template <class T>
Trace<T>::Trace(QString name, T *variable_, QObject *parent)
    : TraceBase(name, parent),
      variable(variable_)
{
    Q_ASSERT(variable);
}

template <class T>
Output* Trace<T>::traceParent() {
    Output *output = dynamic_cast<Output*>(parent());
    return output;
}

template <class T>
Model* Trace<T>::variableParent() {
    Model *model = dynamic_cast<Model*>(variable->parent());
    return model;
}

template <class T>
double Trace<T>::currentValue() {
    QVariant v = variable->toVariant();
    return v.value<double>();
}

} //namespace

#endif
