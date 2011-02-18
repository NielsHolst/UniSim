/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#include "parameter_base.h"
#include "utilities.h"

namespace UniSim{


ParameterBase::ParameterBase(Identifier id, QObject *parent, QString description)
    : QObject(parent), _id(id), _description(description)
{
    setObjectName(id.key());
}

Identifier ParameterBase::id() const {
    return _id;
}

QString ParameterBase::description() const {
    return _description;
}

void ParameterBase::assertUniqueness(Identifier id, QObject *parent) {
    if (!parent) return;
    QList<ParameterBase*> found = seekChildren<ParameterBase*>(id.key(), parent);
    bool isUnique;
    if (found.isEmpty())
        isUnique = true;
    else if (found.size() > 1)
        isUnique = false;
    else
        isUnique = found[0] == this;

    if (!isUnique) {
        QString msg = "Parameter '" + id.label() + "' is not unique in '" + fullName(parent) +"'";
        Q_ASSERT_X(isUnique, "ParameterBase::assertUniqueness", qPrintable(msg));
    }
}


} //namespace


