/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_STRING_CONVERSION_H
#define UNISIM_STRING_CONVERSION_H

#include <QDate>
#include <QString>
#include <QTime>
#include "exception.h"
#include "missing_value.h"

namespace UniSim {

template<class T> T stringToValue(QString s, QObject *concerning = 0);
template<> QString stringToValue<QString>(QString s, QObject *concerning);
template<> double stringToValue<double>(QString s, QObject *concerning);
template<> int stringToValue<int>(QString s, QObject *concerning);
template<> char stringToValue<char>(QString s, QObject *concerning);
template<> bool stringToValue<bool>(QString s, QObject *concerning);
template<> QDate stringToValue<QDate>(QString s, QObject *concerning);
template<> QTime stringToValue<QTime>(QString s, QObject *concerning);

template<class T> QString valueToString(T value);
template<> QString valueToString<QString>(QString value);
template<> QString valueToString<char>(char value);
template<> QString valueToString<bool>(bool value);
template<> QString valueToString<QDate>(QDate value);
template<> QString valueToString<QTime>(QTime value);

template <class T> bool isType(QString s);


template<class T> T stringToValue(QString s, QObject *concerning) {
    QString msg = "stringToValue does not support this type: '%1' of type '%2'";
    throw Exception(msg.arg(s).arg(QVariant(T()).typeName()), concerning);
}

template<class T> QString valueToString(T value) {
    return isMissingValue(value) ? QString("NA") : QVariant(value).toString();
}

template <class T> bool isType(QString s) {
    bool ok = true;
    try {
        stringToValue<T>(s);
    }
    catch (Exception &) {
        ok = false;
    }
    return ok;
}

} //namespace

#endif
