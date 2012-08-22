#include <usbase/parameter.h>
#include <usbase/variable.h>
#include <usbase/utilities.h>          
#include "fixed.h"

namespace UniSim {

Fixed::Fixed(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<QString>("parameters", &parametersAsString, QString("()"), this,
    "Parameters as name value pairs. E.g., @F {((Tavg 22.5)(I 40))}. ");
}

void Fixed::amend() {
    QMap<QString, QString> pList = decodeList<QString, QString>(parametersAsString, this);
    QMapIterator<QString, QString> pa(pList);
    while(pa.hasNext()) {
        pa.next();
        Identifier id = pa.key();
        QString s = pa.value();
        QString desc = "Fixed value";
        if (isType<QDate>(s)) {
            dates << stringToValue<QDate>(s, this);
            new Parameter<QDate>(id, &dates.last(), dates.last(), this, desc);
        }
        else if (isType<QTime>(s)) {
            times << stringToValue<QTime>(s);
            new Parameter<QTime>(id, &times.last(), times.last(), this, desc);
        }
        else if (isType<bool>(s)) {
            bools << stringToValue<bool>(s);
            new Parameter<bool>(id, &bools.last(), bools.last(), this, desc);
        }
        else if (isType<int>(s)) {
            ints << stringToValue<int>(s);
            new Parameter<int>(id, &ints.last(), ints.last(), this, desc);
        }
        else if (isType<double>(s)) {
            doubles << stringToValue<double>(s);
            new Parameter<double>(id, &doubles.last(), doubles.last(), this, desc);
        }
        else {
            strings << s;
            new Parameter<QString>(id, &strings.last(), strings.last(), this, desc);
        }
    }
}


} //namespace
