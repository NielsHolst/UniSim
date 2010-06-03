/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <math.h>
#include "exception.h"
#include "push_variable.h"

namespace UniSim{

PushVariable::PushVariable(Identifier name, double *valuePtr, QObject *parent)
    : QObject(parent), _id(name), _valuePtr(valuePtr)
{
    setObjectName(name.key());
}

double PushVariable::value() const {
    Q_ASSERT(_valuePtr);
    return *_valuePtr;
}

double* PushVariable::valuePtr() {
    return _valuePtr;
}

void PushVariable::setValue(double value) {
    bool ok = true;
    QString msg;
    if (isnan(value))
        msg = "not-a-number";
    if (isinf(value))
        msg = "infinite number";
    if (!ok)
        throw Exception("Variable: " + objectName() +
                        " of " + parent()->objectName() +
                        " set to " + msg);
    Q_ASSERT(_valuePtr);
    *_valuePtr = value;
}

Identifier PushVariable::id() const {
    return _id;
}

} //namespace

