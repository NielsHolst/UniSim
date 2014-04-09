/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_MISSING_VALUE_H
#define UNISIM_MISSING_VALUE_H

#include <math.h>
#include <QDate>
#include <QString>
#include <QTime>
#include <QVariant>
#include "exception.h"


namespace UniSim {

template<class T> T missingValue();
template<> QString missingValue();
template<> double missingValue();
template<> int missingValue();
template<> char missingValue();
template<> QDate missingValue();
template<> QTime missingValue();

template<class T> bool isMissingValue(T value);
template<> bool isMissingValue(double value);
template<> bool isMissingValue(bool value);


template<class T> bool isMissingValue(T value) {
    return value == missingValue<T>();
}


template<class T> T missingValue() {
    QString msg = "Missing value not supported for this type: '%1'";
    throw Exception(msg.arg(QVariant(T()).typeName()));
}

} //namespace

#endif
