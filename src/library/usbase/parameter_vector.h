/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_PARAMETER_VECTOR_H
#define UNISIM_PARAMETER_VECTOR_H

#include "exception.h"
#include "parameter_base.h"
#include "variable_vector.h"

namespace UniSim{

template <class T>
class ParameterVector : public VariableVector<T>, public ParameterBase
{
    //Q_OBJECT
public:
    ParameterVector(Identifier name, const QVector<T> *valuePtr, QObject *parent, QString desc);
    // generic
    void setValueFromString(QString newValue);
    void followRedirection();
    // special
    void setValue(const QVector<T> &newValue);
    void redirectValuePtr(const QVector<T> *redirectedValuePtr);
private:
    // data
    const QVector<T> *redirectedValuePtr;
    // methods

};

template <class T>
ParameterVector<T>::ParameterVector(Identifier id, const QVector<T> *valuePtr, QObject *parent, QString desc)
    :
//      VariableBase(id, parent, desc), // ? otherwise compiler complains
      VariableVector<T>(id, valuePtr, parent, desc),
//      ParameterBase(id, parent, desc),
      redirectedValuePtr(0)
{
}

template <class T>
void ParameterVector<T>::setValue(const QVector<T> &newValue)
{
    *( const_cast<QVector<T>*>(VariableVector<T>::_valuePtr) ) = newValue;
}

template <class T>
void ParameterVector<T>::redirectValuePtr(const QVector<T> *redirectedValuePtr_)
{
    redirectedValuePtr = redirectedValuePtr_;
}

template <class T>
void ParameterVector<T>::setValueFromString(QString newValue)
{
    QString msg = "Value of ParameterVector cannot be set from string ('%1')";
    throw Exception(msg.arg(newValue), this);
}

template <class T>
void ParameterVector<T>::followRedirection() {
    if (redirectedValuePtr)
        setValue(*redirectedValuePtr);
}

} //namespace

#endif

