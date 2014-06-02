/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include <usbase/decode_list.h>
#include <usbase/variable.h>
#include "publish.h"
#include "sum.h"

namespace UniSim{

PUBLISH(Sum)

Sum::Sum(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<QString>("toAdd", &toAdd, QString(), this,
        "List of references to add, e.g. @F {(lion[density] leopard[density])}");
    new Variable<double>("value", &value, this,
        "Sum of values in list");
}

void Sum::initialize() {
    QStringList items = decodeList(toAdd, this);
    int n = items.size();
    values.clear();
    for (int i = 0; i < n; ++i) {
        VariableBase *var = seekOne<Model*,VariableBase*>(items[i]);
        values << var;
    }
}

void Sum::reset() {
    value = 0.;
}

void Sum::update() {
    bool ok = true;
    value = 0;
    for (int i = 0; ok && i < values.size(); ++i)
        value += values.at(i)->toVariant().toDouble(&ok);
    if (!ok)
        throw Exception("One of the variables to be added cannot be converted to a number", this);
}

} //namespace

