/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_PARAMETER_H
#define UNISIM_PARAMETER_H

#include <QDate>
#include <QObject>
#include <QVariant>
#include "exception.h"
#include "parameter_base.h"
#include "utilities.h"

namespace UniSim{

template <class T>
class Parameter : public ParameterBase
{
    // no Q_OBJECT
public:
    Parameter(Identifier name, T *valuePtr, T defaultvalue, QObject *parent, QString desc);
    // generic
    QVariant toVariant() const;
    QString typeId() const;
    void setValueFromString(QString newValue);
    // special
    T value() const;
    T* valuePtr() const;
	void setValue(T newValue);
    void redirectValuePtr(T *valuePtr);
    void followRedirection();

private:

    T *_valuePtr;
    T *redirectedValuePtr;
    T defaultValue;
};

template <class T>
Parameter<T>::Parameter(Identifier id, T *valuePtr, T defaultvalue_, QObject *parent, QString desc)
    :  ParameterBase(id, parent, desc), _valuePtr(valuePtr), redirectedValuePtr(0), defaultValue(defaultvalue_)
{
    assertUniqueness(id, parent);
    Q_ASSERT(_valuePtr);
    setValue(defaultValue);
}

template <class T>
T Parameter<T>::value() const
{
    return *_valuePtr;
}

template <class T>
T* Parameter<T>::valuePtr() const
{
    return _valuePtr;
}

template <class T>
QVariant Parameter<T>::toVariant() const
{
    T val = value();
    return QVariant(val);
}

template <class T>
QString Parameter<T>::typeId() const
{
    QString result = QString(QVariant(T()).typeName()).toLower();
    return (result == "qdate") ? QString("date") : result;
}


template <class T>
void Parameter<T>::setValue(T newValue)
{
    *_valuePtr = newValue;
}

template <class T>
void Parameter<T>::setValueFromString(QString newValue)
{
    try {
        *_valuePtr = stringToValue<T>(newValue);
    }
    catch (Exception &ex) {
        throw Exception(ex.message(), this);
    }
}

template <class T>
void Parameter<T>::redirectValuePtr(T *rederictedPtr) {
    redirectedValuePtr = rederictedPtr;
}


template <class T>
void Parameter<T>::followRedirection() {
    if (redirectedValuePtr)
        *_valuePtr = *redirectedValuePtr;
}

} //namespace

#endif

