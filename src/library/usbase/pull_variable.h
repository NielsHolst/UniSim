/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_PULL_VARIABLE_H
#define UNISIM_PULL_VARIABLE_H

#include <QObject>
#include <QVariant>
#include "pull_variable_base.h"

namespace UniSim{

template <class T>
class PullVariable : public PullVariableBase
{
    // no Q_OBJECT
public:
    PullVariable(Identifier name, const T *valuePtr, QObject *parent, QString desc);
    T value() const;
    const T* valuePtr() const;
    QVariant toVariant() const;

private:
    const T *_valuePtr;
};


template <class T>
PullVariable<T>::PullVariable(Identifier id, const T *valuePtr, QObject *parent, QString desc)
    :  PullVariableBase(id, parent, desc), _valuePtr(valuePtr)
{
}

template <class T>
T PullVariable<T>::value() const
{
    Q_ASSERT(_valuePtr);
    return *_valuePtr;
}

template <class T>
const T* PullVariable<T>::valuePtr() const
{
    return _valuePtr;
}

template <class T>
QVariant PullVariable<T>::toVariant() const
{
    T val = value();
    return QVariant(val);
}

} //namespace

#endif

