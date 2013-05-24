/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include <usbase/utilities.h>          
#include "fixed.h"

namespace UniSim {

namespace {


template <class T>
bool allocateValue(Identifier id, QString s, QMap<Identifier, T> &buf, Fixed *parent) {
    bool ok = isType<T>(s);
    if (ok)
        buf[id] = stringToValue<T>(s, parent);
    return ok;
}

template <class T>
void allocateValues(QVector<T> &values, const QMap<Identifier, T> &buf, Fixed *parent) {
    QMapIterator<Identifier, T> i(buf);
    while(i.hasNext()) {
        i.next();
        values << i.value();
    }
    int ix = -1;
    i.toFront();
    while(i.hasNext()) {
        ++ix; i.next();
        new Parameter<T>(i.key(), &values[ix], values[ix], parent, "Fixed value");
    }
}

} // namespace


Fixed::Fixed(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<QString>("parameters", &parametersAsString, QString("()"), this,
    "Parameters as name value pairs. E.g., @F {((Tavg 22.5)(I 40))}. ");
}

void Fixed::amend() {
    QMap<QString, QString> pList = decodeList<QString, QString>(parametersAsString, this);
    QMapIterator<QString, QString> pa(pList);

    QMap<Identifier, QDate> dateBuf;
    QMap<Identifier, QTime> timeBuf;
    QMap<Identifier, bool> boolBuf;
    QMap<Identifier, int> intBuf;
    QMap<Identifier, double> doubleBuf;
    QMap<Identifier, QString> stringBuf;

    while(pa.hasNext()) {
        pa.next();
        Identifier id = pa.key();
        QString s = pa.value();
        if (allocateValue<QDate>(id, s, dateBuf, this)) continue;
        if (allocateValue<QTime>(id, s, timeBuf, this)) continue;
        if (allocateValue<bool>(id, s, boolBuf, this)) continue;
        if (allocateValue<int>(id, s, intBuf, this)) continue;
        if (allocateValue<double>(id, s, doubleBuf, this)) continue;
        stringBuf[id] = s;
    }
    allocateValues<QDate>(dates, dateBuf, this);
    allocateValues<QTime>(times, timeBuf, this);
    allocateValues<bool>(bools, boolBuf, this);
    allocateValues<int>(ints, intBuf, this);
    allocateValues<double>(doubles, doubleBuf, this);
    allocateValues<QString>(strings, stringBuf, this);
}

} //namespace
