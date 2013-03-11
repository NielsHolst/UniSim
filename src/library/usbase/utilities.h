/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_UTILITIES_H
#define UNISIM_UTILITIES_H

#include <cmath>
#include <cfloat>
#include <iostream>
#include <limits>
#include <typeinfo>
#include <QDate>
#include <QDir>
#include <QList>
#include <QMap>
#include <QObject>
#include <QPluginLoader>
#include <QString>
#include <QStringList>
#include <QTime>
#include <QVariant>
#include "exception.h"
#include "file_locations.h"
#include "identifier.h"

using std::cout;

namespace UniSim {

const double PI = 3.14159265;
const double DEG_TO_RAD = 0.017453293;
const double MAX_EXP = -log(DBL_EPSILON);
const double MIN_EXP = -log(1. - DBL_EPSILON);

class Model;

QFileInfo findNearestFile(QDir home, QString subFolder, QString fileName);

//! @name Mathematics
//@{
double accum(const QVector<double> &x);
void increment(QVector<double> &x, const QVector<double> &toAdd);
void increment(double *x, const double *toAdd, int n);
QVector<double> add(const QVector<double> &x, const QVector<double> &y);
void scale(QVector<double> &x, double factor);

double interpolate(const QMap<int, double> xy, int x);
double pow0(double x, double c);
double negExp(double x);
double divBounded(double x, double y, double bound = std::numeric_limits<double>::max());
double GBFuncResp(double demand, double supply);
int toDayOfYear(int day, int month);
//@}

//! @name String handling
//@{
QString fullName(const QObject *object);
void chopParentheses(QString &s, QObject *concerning = 0);
void chopRightParenthesis(QString &s, QObject *concerning = 0);
QStringList decodeSimpleList(QString parenthesizedList, QObject *concerning = 0);  // deprecated
QStringList decodeList(QString s, QObject *context = 0);
template<class T> QList<T> decodeList(QString s, QObject *context = 0);
template<class T, class U> QMap<T,U> decodeList(QString s, QObject *context = 0);
void splitAtNamespace(QString s, QString *namespacePart, QString *ownNamePart);
QStringList splitParentChildExpression(QString expression);
template <class T> QList< QPair<QString, T> > decodeNameValueList(QString nameValueList, QObject *concerning = 0);

template<class T> T missingValue();
template<> QString missingValue();
template<> double missingValue();
template<> int missingValue();
template<> char missingValue();
template<> QDate missingValue();
template<> QTime missingValue();

template<class T> bool isMissingValue(T value);
template<> bool isMissingValue(bool value);

template<class T> T stringToValue(QString s, QObject *concerning = 0);
template<> QString stringToValue<QString>(QString s, QObject *concerning);
template<> double stringToValue<double>(QString s, QObject *concerning);
template<> int stringToValue<int>(QString s, QObject *concerning);
template<> char stringToValue<char>(QString s, QObject *concerning);
template<> bool stringToValue<bool>(QString s, QObject *concerning);
template<> QDate stringToValue<QDate>(QString s, QObject *concerning);
template<> QTime stringToValue<QTime>(QString s, QObject *concerning);

template<class T> QString valueToString(T value);
template<> QString valueToString<char>(char value);
template<> QString valueToString<bool>(bool value);
template<> QString valueToString<QDate>(QDate value);
template<> QString valueToString<QTime>(QTime value);

template <class T> bool isType(QString s);
//@}

//! @name Testing
//@{
void writeObjectTree(QObject *root, int level = 0);
void writeStandardTestFile(QString filePath);
//@}

// ========================
// Template implementations

template<class T> bool isMissingValue(T value) {
    return value == missingValue<T>();
}

template<class T> T missingValue() {
//    static_assert( false, "that was false" );
    QString msg = "Missing value not supported for this type: '%1'";
    throw Exception(msg.arg(QVariant(T()).typeName()));
}

template<class T> T stringToValue(QString s, QObject *concerning) {
//    static_assert( false, "that was false" );
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

//
// String handling
//

template<class T>
QList<T> decodeList(QString s, QObject *context) {
    QList<T> result;
    QStringList strings = decodeList(s, context);
    for (int i = 0; i < strings.size(); ++i) {
        T value = stringToValue<T>(strings[i], context);
        result.append(value);
    }
    return result;
}

template<class T, class U>
QMap<T,U> decodeList(QString s, QObject *context) {
    QMap<T,U> result;
    QStringList pairs = decodeList(s, context);
    for (int i = 0; i < pairs.size(); ++i) {
        QStringList pair = decodeList(pairs[i]);
        if (pair.size() != 2)
            throw Exception("(name value) pair expected, got: \"" + pairs[i] + "\"", context);
        T key = stringToValue<T>(pair[0], context);
        U value = stringToValue<U>(pair[1], context);
        if (result.contains(key))
            throw Exception("Name must be unique in list, found \"" + pair[0] + "\" twice");
        result[key] = value;
    }
    return result;
}

template <class T>
QList< QPair<QString, T> > decodeNameValueList(QString nameValueList, QObject *concerning) {
    QList< QPair<QString, T> > result;
    QString s = nameValueList.simplified();
    s = s.trimmed();
    if (s.size() == 0) throw Exception("Name-value list is empty", concerning);
    if (s.left(1) != "(") throw Exception("Name-value list must begin with '(': " + s, concerning);

    QStringList parts = s.split("(");
    for (int i = 0; i < 2; ++i) {
        if (parts[i].size() > 0 && parts[i].left(1) != " ")
            throw UniSim::Exception("Name-value list must begin with two left parentheses: " + s, concerning);
    }

    if (parts.size() < 3)
        throw UniSim::Exception("Name-value list is incomplete: " + s, concerning);

    for (int i = 2; i< parts.size(); ++i) {
        QString part = parts[i].trimmed();
        chopRightParenthesis(part, concerning);
        if (i == parts.size() - 1)
            chopRightParenthesis(part, concerning);

        QStringList strPair = part.split(" ");
        bool ok = strPair.size() == 2;
        if (ok) {
            QPair<QString, T> pair;
            pair.first = strPair[0];
            pair.second = stringToValue<T>(strPair[1], concerning);
            result.append(pair);
        }

        if (!ok)
            throw UniSim::Exception("Name-value list must contain (name value) pairs: " + s, concerning);
    }
    return result;
}

} //namespace

#endif
