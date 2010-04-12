/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_PARAMETERS_H
#define UNISIM_PARAMETERS_H

#include <QDate>
#include <QMap>
#include <QStringList>
#include <QTextStream>
#include <QVariant>
#include "exception.h"
#include "identifier.h"

namespace UniSim{

class SimulationMaker;
class TestParameters;

class Parameters 
{
	friend class SimulationMaker;
	friend class TestParameters;
	
private:
    struct Parameter {
		typedef enum {String, Number, Date, Bool} StringValueType;
        Parameter();
	    
		QDate validatedDate() const;
        StringValueType stringValueType() const;
        bool canConvertStringValue(QVariant::Type qt) const;
		
        QString stringValue;
		QVariant::Type typePointedTo; 
        void *valuePtr;
    };

public:
	Parameters();
	
    //! Sets parameter value
    /*!
        It is asserted that valuePtr is valid. If name refers to an existing parameter then the
        value of that is copied to *valuePtr, otherwise defaultValue is copied,

        Parameters are typically set up during initialize(). The value can be retrieved via parameter().
    */
    template <class T> void setParameter(Identifier name, T *valuePtr, T defaultValue);


    //! Changes parameter value
    /*!
        The value of the parameter is changed.

        An Exception is thrown if name does not exist or the type T is not convertible to the
        existing value.
    */
    template <class T> void changeParameter(Identifier name, T value);
    
    //! Returns parameter value
    /*!
        An Exception is thrown if name does not exist.
     */
    template <class T> T parameter(Identifier name) const;

    QString parameterAsString(Identifier name) const;

    void initParameter(Identifier name, QString value);
    Identifiers allParameters() const;

private:
    static QDate validatedDate(QString s);
    template <class T> T* validatedValuePtr(Identifier name) const;
    QMap<Identifier, Parameter> _parameters;
};

template <class T> QVariant::Type getType() {
	return QVariant(T()).type();
}

template <class T> void Parameters::setParameter(Identifier name, T *valuePtr, T defaultValue) {
    Q_ASSERT(valuePtr);
		
    if (!_parameters.contains(name)) {
    	_parameters[name] = Parameters::Parameter();
    	*valuePtr = defaultValue;
        QString s;
        QTextStream str(&s);
        str << defaultValue;
        _parameters[name].stringValue = s;
    }
   else {
   	   Parameter &param(_parameters[name]);
	   QVariant::Type wantedType = getType<T>();
		if (!param.canConvertStringValue(wantedType)) 
			throw Exception("Cannot convert parameter '" + param.stringValue + "' to type " + QVariant::typeToName(wantedType));
		
		*valuePtr = QVariant(param.stringValue).value<T>();
    }
	
    _parameters[name].valuePtr = (void *) valuePtr;
    _parameters[name].typePointedTo = getType<T>();
}

template <> void Parameters::setParameter<QDate>(Identifier name, QDate *valuePtr, QDate defaultValue);
template <> void Parameters::setParameter<bool>(Identifier name, bool *valuePtr, bool defaultValue);

template <class T> void Parameters::changeParameter(Identifier name, T value) {
    T* valuePtr = validatedValuePtr<T>(name);
    *valuePtr = value;
}

template <class T> T Parameters::parameter(Identifier name) const {
    T* valuePtr;
    T value;
    // get parameter value from value pointed to
    try {
        valuePtr = validatedValuePtr<T>(name);
        value = *valuePtr;
    }
    // or else try converting it from its string value if it exists
    catch (UniSim::Exception &ex) {
        if (_parameters.contains(name))
            value = QVariant(_parameters[name].stringValue).value<T>();
        else
            throw;
    }
    return value;
}

template <class T> T* Parameters::validatedValuePtr(Identifier name) const {
    if (!_parameters.contains(name))
        throw Exception("Attempt to access non-existing parameter with key '" +
                        name.key() + "'");
		
	if (!_parameters[name].valuePtr)	
        throw Exception("Attempt to access parameter named '" + name.label() +
                        "' but its value pointer has not been set previously.");

	QVariant::Type wantedType = _parameters[name].typePointedTo;
	if ( wantedType != getType<T>())
        throw Exception("Attempt to access parameter named '" + name.label() +
                        "' but its type: " + QVariant::typeToName(wantedType) +
                        " does not match that of the new value:" + QVariant::typeToName(getType<T>()));
								    
	return (T*) _parameters[name].valuePtr;
}

} //namespace

#endif
