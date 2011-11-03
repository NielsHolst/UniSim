/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_PARAMETER_FILE_H
#define UNISIM_PARAMETER_FILE_H

#include <iostream>
#include <QFile>
#include <QHash>
#include <QStringList>
#include "exception.h"
#include "parameter_file_base.h"
#include "utilities.h"

namespace UniSim{

//
// One row key
//

template <class Key1>
class ParameterFile1D : public ParameterFileBase
{
    // no Q_OBJECT
public:
    ParameterFile1D(Identifier name, QString defaultFilePath, QObject *parent, QString desc);
    QString rowKey(QStringList keys) const;
    template <class ValueType>
        ValueType value(Key1 rowKey1, QString columnKey);
};

template <class Key1>
ParameterFile1D<Key1>::ParameterFile1D(Identifier id, QString defaultFilePath, QObject *parent, QString desc)
    :  ParameterFileBase(id, defaultFilePath, parent, desc)
{
}

template <class Key1>
QString ParameterFile1D<Key1>::rowKey(QStringList keys) const {
    return Identifier(QStringList(keys.mid(0,1)).join("-")).key();
}

template<class Key1> template<class ValueType>
ValueType ParameterFile1D<Key1>::value(Key1 rowKey1, QString columnKey) {
    QStringList keys;
    keys << valueToString(rowKey1);
    QString strValue = stringValue(rowKey(keys), columnKey);
    ValueType result;
    try {
        result = stringToValue<ValueType>(strValue);
    }
    catch (Exception &ex) {
        QString msg("ParameterFile1D '%1' column '%2' in file '%3': value '%4' is of wrong type\n");
        msg = msg.arg(id().label()).arg(columnKey).arg(filePath()).arg(strValue);
        throw Exception(msg + ex.message(), this);
    }
    return result;
}

//
// Two row keys
//

template <class Key1, class Key2>
class ParameterFile2D : public ParameterFileBase
{
    // no Q_OBJECT
public:
    ParameterFile2D(Identifier name, QString defaultFilePath, QObject *parent, QString desc);
    QString rowKey(QStringList keys) const;
    template <class ValueType>
        ValueType value(Key1 rowKey1, Key2 rowKey2, QString columnKey);
};

template <class Key1, class Key2>
ParameterFile2D<Key1, Key2>::ParameterFile2D(Identifier id, QString defaultFilePath, QObject *parent, QString desc)
    :  ParameterFileBase(id, defaultFilePath, parent, desc)
{
}

template <class Key1, class Key2>
QString ParameterFile2D<Key1, Key2>::rowKey(QStringList keys) const {
    return Identifier(QStringList(keys.mid(0,2)).join("-")).key();
}

template<class Key1, class Key2> template<class ValueType>
ValueType ParameterFile2D<Key1, Key2>::value(Key1 rowKey1, Key2 rowKey2, QString columnKey) {
    QStringList keys;
    keys << valueToString(rowKey1) << valueToString(rowKey2);
    QString strValue = stringValue(rowKey(keys), columnKey);
    ValueType result;
    try {
        result = stringToValue<ValueType>(strValue);
    }
    catch (Exception &ex) {
        QString msg("ParameterFile2D '%1' column '%2' in file '%3': value '%4' is of wrong type\n");
        msg = msg.arg(id().label()).arg(columnKey).arg(filePath()).arg(strValue);
        throw Exception(msg + ex.message(), this);
    }
    return result;
}

//
// Three row keys
//

template <class Key1, class Key2, class Key3>
class ParameterFile3D : public ParameterFileBase
{
    // no Q_OBJECT
public:
    ParameterFile3D(Identifier name, QString defaultFilePath, QObject *parent, QString desc);
    QString rowKey(QStringList keys) const;
    template <class ValueType>
        ValueType value(Key1 rowKey1, Key2 rowKey2, Key3 rowKey3, QString columnKey);
};

template <class Key1, class Key2, class Key3>
ParameterFile3D<Key1, Key2, Key3>::ParameterFile3D(Identifier id, QString defaultFilePath, QObject *parent, QString desc)
    :  ParameterFileBase(id, defaultFilePath, parent, desc)
{
}

template <class Key1, class Key2, class Key3>
QString ParameterFile3D<Key1, Key2, Key3>::rowKey(QStringList keys) const {
    return Identifier(QStringList(keys.mid(0,3)).join("-")).key();
}

template<class Key1, class Key2, class Key3> template<class ValueType>
ValueType ParameterFile3D<Key1, Key2, Key3>::value(Key1 rowKey1, Key2 rowKey2, Key3 rowKey3, QString columnKey) {
    QStringList keys;
    keys << valueToString(rowKey1) << valueToString(rowKey2) << valueToString(rowKey3);
    QString strValue = stringValue(rowKey(keys), columnKey);
    ValueType result;
    try {
        result = stringToValue<ValueType>(strValue);
    }
    catch (Exception &ex) {
        QString msg("ParameterFile3D '%1' column '%2' in file '%3': value '%4' is of wrong type\n");
        msg = msg.arg(id().label()).arg(columnKey).arg(filePath()).arg(strValue);
        throw Exception(msg + ex.message(), this);
    }
    return result;
}

//
// Four row keys
//

template <class Key1, class Key2, class Key3, class Key4>
class ParameterFile4D : public ParameterFileBase
{
    // no Q_OBJECT
public:
    ParameterFile4D(Identifier name, QString defaultFilePath, QObject *parent, QString desc);
    QString rowKey(QStringList keys) const;
    template <class ValueType>
        ValueType value(Key1 rowKey1, Key2 rowKey2, Key3 rowKey3, Key4 rowKey4, QString columnKey);
};

template <class Key1, class Key2, class Key3, class Key4>
ParameterFile4D<Key1, Key2, Key3, Key4>::ParameterFile4D(Identifier id, QString defaultFilePath, QObject *parent, QString desc)
    :  ParameterFileBase(id, defaultFilePath, parent, desc)
{
}

template <class Key1, class Key2, class Key3, class Key4>
QString ParameterFile4D<Key1, Key2, Key3, Key4>::rowKey(QStringList keys) const {
    return Identifier(QStringList(keys.mid(0,4)).join("-")).key();
}

template<class Key1, class Key2, class Key3, class Key4> template<class ValueType>
ValueType ParameterFile4D<Key1, Key2, Key3, Key4>::value(Key1 rowKey1, Key2 rowKey2, Key3 rowKey3, Key4 rowKey4, QString columnKey) {
    QStringList keys;
    keys << valueToString(rowKey1) << valueToString(rowKey2) << valueToString(rowKey3) << valueToString(rowKey4);
    QString strValue = stringValue(rowKey(keys), columnKey);
    ValueType result;
    try {
        result = stringToValue<ValueType>(strValue);
    }
    catch (Exception &ex) {
        QString msg("ParameterFile4D '%1' column '%2' in file '%3': value '%4' is of wrong type\n");
        msg = msg.arg(id().label()).arg(columnKey).arg(filePath()).arg(strValue);
        throw Exception(msg + ex.message(), this);
    }
    return result;
}

} //namespace

#endif

