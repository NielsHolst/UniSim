/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_STRING_MAP_H
#define UNISIM_STRING_MAP_H

#include <QMap>
#include <QString>
#include <QStringList>
#include "exception.h"

#define StringMapEntry(map, tag) map [ #tag ] = tag

namespace UniSim{

class NamedObject;

template <class T>
class StringMap : public QMap<QString,T>
{
public:
    StringMap();
    QStringList validKeys() const;
    T seek(QString key, NamedObject *context=0);
};

template <class T>
StringMap<T>::StringMap()
    : QMap<QString,T>()
{
}

template <class T>
QStringList StringMap<T>::validKeys() const {
    return QStringList(QMap<QString, T>::keys());
}

template <class T>
T StringMap<T>::seek(QString key, NamedObject *context) {
    QString k = key.toLower();
    if (!QMap<QString, T>::contains(k)) {
        QString msg = "Unknown key in list: '%1'. Only these keys are valid: '%2'";
        throw Exception(msg.arg(key).arg(validKeys().join(",")), context);
    }
    else
        return QMap<QString, T>::value(k);
}

} //namespace

#endif

