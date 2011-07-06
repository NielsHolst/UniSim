/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_PUSH_VARIABLE_H
#define UNISIM_PUSH_VARIABLE_H

#include <QDate>
#include <QDateTime>
#include <QObject>
#include <QTime>
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
    QString typeId() const;
    void setValue(T newValue);
    void addValue(T addition);
    void resetValue();

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
QString PushVariable<T>::typeId() const
{
    QString result = QString(QVariant(T()).typeName()).toLower();
    return (result == "qdate") ? QString("date") : result;
}

template <class T>
void PushVariable<T>::setValue(T newValue)
{
    Q_ASSERT(_valuePtr);
    *_valuePtr = newValue;
}

namespace {
    template <class T> void add(T *a, T b) { *a += b; }
    template <> void add<bool>(bool *a, bool b) { *a = *a || b; }
    template <> void add<QDateTime>(QDateTime *a, QDateTime b) { *a = b; }
    template <> void add<QDate>(QDate *a, QDate b) { *a = b; }
    template <> void add<QTime>(QTime *a, QTime b) { *a = b; }
}

template <class T>
void PushVariable<T>::addValue(T addition)
{
    Q_ASSERT(_valuePtr);
    add(_valuePtr, addition);
}

template <class T>
void PushVariable<T>::resetValue()
{
    Q_ASSERT(_valuePtr);
    *_valuePtr = T();
}
} //namespace

#endif

