/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_PARAMETER_H
#define UNISIM_PARAMETER_H

#include "exception.h"
#include "parameter_base.h"
#include "variable.h"

namespace UniSim{

template <class T>
class Parameter : public Variable<T>, public ParameterBase
{
    // no Q_OBJECT
public:
    Parameter(Identifier name, T *valuePtr, T defaultvalue, QObject *parent, QString desc);
    // generic
    void setValueFromString(QString newValue);
    void followRedirection();
    // special
    void setValue(T newValue);
    void redirectValuePtr(const T *redirectedValuePtr);
private:
    // data
    T defaultValue;
    const T *redirectedValuePtr;
    // methods

};

template <class T>
Parameter<T>::Parameter(Identifier id, T *valuePtr, T defaultvalue_, QObject *parent, QString desc)
    :
      VariableBase(id, parent, desc), // ? otherwise compiler complains
      Variable<T>(id, valuePtr, parent, desc),
      ParameterBase(id, parent, desc),
      defaultValue(defaultvalue_),
      redirectedValuePtr(0)
{
    setValue(defaultValue);
}

template <class T>
void Parameter<T>::setValue(T newValue)
{
    *( const_cast<T*>(Variable<T>::_valuePtr) ) = newValue;
}

template <class T>
void Parameter<T>::redirectValuePtr(const T *redirectedValuePtr_)
{
    redirectedValuePtr = redirectedValuePtr_;
}

template <class T>
void Parameter<T>::setValueFromString(QString newValue)
{
    try {
        setValue( stringToValue<T>(newValue) );
    }
    catch (Exception &ex) {
        throw Exception(ex.message(), this);
    }
}

template <class T>
void Parameter<T>::followRedirection() {
    if (redirectedValuePtr)
        setValue(*redirectedValuePtr);
}

} //namespace

#endif

