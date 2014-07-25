/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "string_conversion.h"

namespace UniSim {

template<> QString stringToValue<QString>(QString s_, QObject *) {
    return s_;
}

template<> double stringToValue<double>(QString s_, QObject *concerning) {
    QString s = s_.trimmed();
    if (s == missingValue<QString>())
        return missingValue<double>();
    bool ok;
    double value = s.toDouble(&ok);
    if (!ok) {
        QString msg = "Cannot convert '" + s + "' to double";
        throw Exception(msg, concerning);
    }
    return value;
}

template<> int stringToValue<int>(QString s_, QObject *concerning) {
    QString s = s_.trimmed();
    if (s == missingValue<QString>())
        return missingValue<int>();
    bool ok;
    int value = s.toInt(&ok);
    if (!ok) {
        QString msg = "Cannot convert '" + s + "' to int";
        throw Exception(msg, concerning);
    }
    return value;
}

template<> char stringToValue<char>(QString s_, QObject *concerning) {
    QString s = s_.trimmed();
    if (s == missingValue<QString>())
        return missingValue<char>();
    if (s.size() != 1) {
        QString msg = "Cannot convert '" + s + "' to char";
        throw Exception(msg, concerning);
    }
    return s[0].toLatin1();
}

template<> bool stringToValue<bool>(QString s_, QObject *concerning) {
    QString s = s_.trimmed().toLower();
    bool value;
    if (s=="yes" || s=="true")
        value = true;
    else if (s=="no" || s=="false")
        value = false;
    else {
        QString msg = "Cannot convert '" + s + "' to bool";
        throw Exception(msg, concerning);
    }
    return value;
}

template<> QDate stringToValue<QDate>(QString s_, QObject *concerning) {
    QString s = s_.trimmed();
    if (s == missingValue<QString>())
        return missingValue<QDate>();
    QDate date = QDate::fromString(s, "d/M/yyyy");
    if (!date.isValid())
        date = QDate::fromString(s, "d.M.yyyy");
    if (!date.isValid())
        date = QDate::fromString(s, "d-M-yyyy");
    if (!date.isValid())
        date = QDate::fromString(s, "yyyy/M/d");
    if (!date.isValid())
        date = QDate::fromString(s, "yyyy.M.d");
    if (!date.isValid())
        date = QDate::fromString(s, "yyyy-M-d");
    if (!date.isValid()) {
        QString msg = "Cannot convert '" + s + "' to a date";
        throw Exception(msg, concerning);
    }
    return date;
}

template<> QTime stringToValue<QTime>(QString s_, QObject *concerning) {
    QString s = s_.trimmed();
    if (s == missingValue<QString>())
        return missingValue<QTime>();
    QTime time = QTime::fromString(s, "h:m:s");
    if (!time.isValid())
            time = QTime::fromString(s, "h:m");
    if (!time.isValid()) {
        QString msg = "Cannot convert '" + s + "' to a time";
        throw Exception(msg, concerning);
    }
    return time;
}

template<> QString valueToString<QString>(QString value) {
    return value;
}

template<> QString valueToString<char>(char value) {
    return isMissingValue(value) ? QString("NA") :QString(value);
}

template<> QString valueToString<bool>(bool value) {
    return value ? "yes" : "no";
}

template<> QString valueToString<QDate>(QDate value) {
    return isMissingValue(value) ? QString("NA") :value.toString("d/M/yyyy");
}

template<> QString valueToString<QTime>(QTime value) {
    return isMissingValue(value) ? QString("NA") :value.toString("0:0:0");
}


} // namespace
