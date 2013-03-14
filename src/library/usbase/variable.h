/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_VARIABLE_H
#define UNISIM_VARIABLE_H

#include <QObject>
#include <QVariant>
#include "utilities.h"
#include "variable_base.h"

namespace UniSim{

template <class T>
class Variable : public VariableBase
{
    //Q_OBJECT
public:
    Variable(Identifier name, const T *valuePtr, QObject *parent, QString desc);
    // generic
    QVariant toVariant() const;
    QString toString() const;
    QString typeId() const;
    // special
    T value() const;
    const T* valuePtr() const;

protected:
    const T *_valuePtr; // Derived class Parameter casts away const to change the value pointed to
};


template <class T>
Variable<T>::Variable(Identifier id, const T *valuePtr, QObject *parent, QString desc)
    :  VariableBase(id, parent, desc), _valuePtr(valuePtr)
{
    Q_ASSERT(_valuePtr);
}

template <class T>
QVariant Variable<T>::toVariant() const
{
    T val = value();
    return QVariant(val);
}

template <class T>
QString Variable<T>::toString() const
{
    T val = value();
    return valueToString(val);
}

template <class T>
QString Variable<T>::typeId() const
{
    QString result = QString(QVariant(T()).typeName()).toLower();
    return result.startsWith("q") ? result.remove(0,1) : result;
}

template <class T>
T Variable<T>::value() const
{
    Q_ASSERT(_valuePtr);
    return *_valuePtr;
}

template <class T>
const T* Variable<T>::valuePtr() const
{
    return _valuePtr;
}

} //namespace

#endif

