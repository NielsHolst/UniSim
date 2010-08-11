/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_PUSH_VARIABLE_H
#define UNISIM_PUSH_VARIABLE_H

#include <QObject>
#include <QVariant>
#include "push_variable_base.h"

namespace UniSim{

template <class T>
class PushVariable : public PushVariableBase
{
    // no Q_OBJECT
public:
    PushVariable(Identifier name, T *valuePtr, QObject *parent, QString desc);
    T value() const;
    T* valuePtr() const;
    QVariant toVariant() const;
	void setValue(T newValue);

private:
    T *_valuePtr;
};


template <class T>
PushVariable<T>::PushVariable(Identifier id, T *valuePtr, QObject *parent, QString desc)
    :  PushVariableBase(id, parent, desc), _valuePtr(valuePtr)
{
    Q_ASSERT(_valuePtr);
}

template <class T>
T PushVariable<T>::value() const
{
    Q_ASSERT(_valuePtr);
    return *_valuePtr;
}

template <class T>
T* PushVariable<T>::valuePtr() const
{
    return _valuePtr;
}

template <class T>
QVariant PushVariable<T>::toVariant() const
{
    T val = value();
    return QVariant(val);
}

template <class T>
void PushVariable<T>::setValue(T newValue)
{
    Q_ASSERT(_valuePtr);
    *_valuePtr = newValue;
}

} //namespace

#endif

