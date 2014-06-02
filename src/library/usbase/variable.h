/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_VARIABLE_H
#define UNISIM_VARIABLE_H

#include <QObject>
#include <QVariant>
#include "named_object.h"
#include "string_conversion.h"
#include "variable_base.h"

namespace UniSim{

template <class T>
class Variable : public VariableBase
{
public:
    Variable(Identifier name, T *valuePtr, NamedObject *parent, QString);
    // generic
    void initializeValue();
    void resetValue();
    QVariant toVariant() const;
    QString toString() const;
    QString typeId() const;
    void resolveReference();
    void followReference();
    // special
    void initialize(T value);
    void reset(T value);
    T value() const;
    const T* valuePtr() const;

protected:
    T *_valuePtr;

private:
    T valueAtInitialize, valueAtReset;
    const T *referencedValuePtr;
};


template <class T>
Variable<T>::Variable(Identifier id, T *valuePtr, NamedObject *parent, QString)
    :  VariableBase(id, parent), _valuePtr(valuePtr)
{
    Q_ASSERT(_valuePtr);
}

template <class T>
void Variable<T>::initializeValue() {
    *_valuePtr = valueAtInitialize;
}

template <class T>
void Variable<T>::resetValue() {
    *_valuePtr = valueAtReset;
}

template <class T>
void Variable<T>::initialize(T value) {
    valueAtInitialize = value;
}

template <class T>
void Variable<T>::reset(T value) {
    valueAtReset = value;
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
void Variable<T>::resolveReference() {
    if (!isReference()) return;
    try {
        referencedValuePtr =
                _parent
                ->seekOne<NamedObject*, Variable<T>*>(_reference)
                ->valuePtr();
    }
    catch (Exception &) {
        throw Exception(_reference.notFoundMessage(), _parent);
    }
}

template <class T>
void Variable<T>::followReference() {
    Q_ASSERT(referencedValuePtr != 0);
    *_valuePtr = *referencedValuePtr;
}

template <class T>
T Variable<T>::value() const
{
    return *_valuePtr;
}

template <class T>
const T* Variable<T>::valuePtr() const
{
    return _valuePtr;
}

} //namespace

#endif

