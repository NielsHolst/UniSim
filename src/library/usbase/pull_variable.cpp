/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "pull_variable.h"

namespace UniSim{

PullVariable::PullVariable(Identifier name, const double *valuePtr, QObject *parent, QString desc_)
    : QObject(parent), _id(name), _valuePtr(valuePtr), desc(desc_)
{
    setObjectName(name.key());
}

double PullVariable::value() const {
    Q_ASSERT(_valuePtr);
    return *_valuePtr;
}

const double* PullVariable::valuePtr() const {
    return _valuePtr;
}

Identifier PullVariable::id() const {
    return _id;
}

QString PullVariable::description() const {
    return desc;
}

} //namespace

