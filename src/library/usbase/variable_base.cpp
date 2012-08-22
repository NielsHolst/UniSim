/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "utilities.h"
#include "variable_base.h"

namespace UniSim{

VariableBase::VariableBase(Identifier id, QObject *parent, QString description)
    : QObject(parent), _id(id), _description(description)
{
    setObjectName(id.key());
    assertUniqueness();
}

Identifier VariableBase::id() const {
    return _id;
}

QString VariableBase::description() const {
    return _description;
}

void VariableBase::assertUniqueness() {
    QList<VariableBase*> found = seekChildren<VariableBase*>(id().key(), parent());
    bool isUnique = true;
    for (int i=0; isUnique && (i < found.size()); ++i) {
        isUnique = (found[i] == this);
    }
    if (!isUnique) {
        QString msg = "Variable '" + id().label() + "' is not unique in '" + fullName(parent()) +"'";
        Q_ASSERT_X(isUnique, "VariableBase::assertUniqueness", qPrintable(msg));
    }
}

} //namespace


