/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include "identifier.h"
#include "parameters.h"

/*! \class UniSim::Parameters
    \brief The %Parameters class provides parameters as name-value pairs

    A parameter is named by an Identifier. I holds a value that can be a C++ number or bool type,
    a QString or QDate.

    The class is used as a mix-in through (multiple) inheritance. It holds no virtual functions.
*/


namespace UniSim{

Parameters::Parameter::Parameter()
    : stringValue(), typePointedTo(QVariant::Invalid), valuePtr(0)
{
}

//! Returns a list of all parameter identifiers
QList<Identifier> Parameters::allParameters() const {
    return _parameters.keys();
}

//! Returns parameter value as string
/*!
    An Exception is thrown if name does not exist.
 */
QString Parameters::parameterAsString(Identifier name) const {
    if (!_parameters.contains(name))
        throw Exception("Attempt to access non-existing parameter with key '" + name.key() +"'");
    return _parameters[name].stringValue;
}

//! Initialized parameter giving at a string value
/*!
    A parameter is creates and initialized with the string value. The parameter can later be linked
    to a value pointer via setParameter(). The string value must then be covertible to the type of
    the value pointer.

    If name already exists an Exception is thrown.
*/
void Parameters::initParameter(Identifier name, QString value) {

    if (_parameters.contains(name))
        throw Exception("Parameter name occurs twice, must be unique for a component: " + name.label());

    _parameters[name].stringValue = value.trimmed();
}

QDate Parameters::Parameter::validatedDate() const {
	if (stringValueType() != Date)
		throw Exception("Date is not in valid format: '" + stringValue + "' should be 'dd/mm/yyyy', e.g. '3/12/2008");
	return QDate::fromString(stringValue.trimmed(), "d/M/yyyy");
}

Parameters::Parameter::StringValueType Parameters::Parameter::stringValueType() const {
	bool ok;
	stringValue.toDouble(&ok);
	if (ok) return Number;
	
	QDate date = QDate::fromString(stringValue, "d/M/yyyy");
	if (date.isValid()) return Date;
	
	if (stringValue.toLower() == "yes" || stringValue.toLower() == "no") return Bool;
	
	return String;
}
        
bool Parameters::Parameter::canConvertStringValue(QVariant::Type qt) const {
	StringValueType svt = stringValueType();
	bool can = false;
	switch (qt) {
		case QVariant::Bool :
			can = svt==Bool;
			break;
			
		case QVariant::Char :
		case QVariant::String :
			can = svt==String;
			break;
			
		case QVariant::Date :
			can = svt==Date;
			break;
			
		case QVariant::Double :
		case QVariant::Int :
		case QVariant::LongLong :
		case QVariant::UInt :
		case QVariant::ULongLong :
			can = svt==Number;
			break;
			
		default:
			throw Exception(QString("Unknown value type for parameter: ") + QVariant::typeToName(qt));
	}
	Q_ASSERT(!can || QVariant(stringValue).canConvert(qt));
	return can;
}
    	    
Parameters::Parameters()
{
}

template <> void Parameters::setParameter<QDate>(Identifier name, QDate *valuePtr, QDate defaultValue) {
    Q_ASSERT(valuePtr);
		
    if (!_parameters.contains(name)) {
    	_parameters[name] = Parameters::Parameter();
       *valuePtr = defaultValue;
       _parameters[name].stringValue = defaultValue.toString("d/M/yyyy");
   }
   else {
	   *valuePtr = _parameters[name].validatedDate();
	}
	
    _parameters[name].valuePtr = (void *) valuePtr;
    _parameters[name].typePointedTo = QVariant::Date;
}

template <> void Parameters::setParameter<bool>(Identifier name, bool *valuePtr, bool defaultValue) {
    Q_ASSERT(valuePtr);
		
    if (!_parameters.contains(name)) {
    	_parameters[name] = Parameters::Parameter();
       *valuePtr = defaultValue;
       _parameters[name].stringValue = defaultValue ? "yes" : "no";
   }
   else {
	   	QString value = _parameters[name].stringValue.toLower();
		if (value == "yes")
	   	   *valuePtr = true;
		else if (value == "no")
	   	   *valuePtr = false;
	   	 else
	   	 	throw Exception("Boolean value should have value 'yes' or 'no'. Value was: " + value);
	}
	
    _parameters[name].valuePtr = (void *) valuePtr;
    _parameters[name].typePointedTo = QVariant::Bool;
}

} //namespace

