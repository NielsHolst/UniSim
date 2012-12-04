/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_VARIABLE_VECTOR_H
#define UNISIM_VARIABLE_VECTOR_H

#include <QObject>
#include <QVariant>
#include <QVector>
#include "utilities.h"
#include "variable_base.h"

namespace UniSim{

template <class T>
class VariableVector : public virtual VariableBase
{
    //Q_OBJECT
public:
    VariableVector(Identifier name, const QVector<T> *valuePtr, QObject *parent, QString desc);
    // generic
    QVariant toVariant() const;
    QString toString() const;
    QString typeId() const;
    // special
    const QVector<T>& value() const;
    const QVector<T>* valuePtr() const;

protected:
    const QVector<T> *_valuePtr; // Derived class Parameter casts away const to change the value pointed to
};


template <class T>
VariableVector<T>::VariableVector(Identifier id, const QVector<T> *valuePtr, QObject *parent, QString desc)
    :  VariableBase(id, parent, desc), _valuePtr(valuePtr)
{
    Q_ASSERT(_valuePtr);
}

template <class T>
QVariant VariableVector<T>::toVariant() const
{
    const T* p = _valuePtr->data();
    int n = _valuePtr->size();
    T sum = 0;
    for (int i = 0; i < n; ++i, ++p)
        sum += *p;
    return QVariant(sum);
}

template <class T>
QString VariableVector<T>::toString() const
{
    QVariant var = toVariant();
    return var.toString();
}

template <class T>
QString VariableVector<T>::typeId() const
{
    QString result = QString(QVariant(T()).typeName()).toLower();
    if (result.startsWith("q") )
        result = result.remove(0,1);
    return "QVector<" + result  + ">";
}

template <class T>
const QVector<T>& VariableVector<T>::value() const
{
    Q_ASSERT(_valuePtr);
    return *_valuePtr;
}

template <class T>
const QVector<T> *VariableVector<T>::valuePtr() const
{
    return _valuePtr;
}

} //namespace

#endif

