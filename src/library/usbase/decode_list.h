/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_DECODE_LIST_H
#define UNISIM_DECODE_LIST_H

#include <QList>
#include <QMap>
#include <QString>
#include <QStringList>
#include "exception.h"
#include "string_conversion.h"

namespace UniSim {

void chopParentheses(QString &s, QObject *concerning = 0);
void chopRightParenthesis(QString &s, QObject *concerning = 0);
QStringList decodeSimpleList(QString parenthesizedList, QObject *concerning = 0);  // deprecated
QStringList decodeList(QString s, QObject *context = 0);
template<class T> QList<T> decodeList(QString s, QObject *context = 0);
template<class T, class U> QMap<T,U> decodeList(QString s, QObject *context = 0);
template <class T> QList< QPair<QString, T> > decodeNameValueList(QString nameValueList, QObject *concerning = 0);


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
