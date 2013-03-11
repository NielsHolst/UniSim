/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "exception.h"
#include "named_object.h"
#include "utilities.h"
#include "variable_base.h"

namespace UniSim{

VariableBase::VariableBase(Identifier id, QObject *parent, QString description)
    : QObject(parent),  _id(id), _description(description)
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
    NamedObject *par = dynamic_cast<NamedObject*>(parent());
    if (!par)
        return;

    QList<VariableBase*> found = par->seekChildren<VariableBase*>(id().key());
    bool isUnique = true;
    for (int i=0; isUnique && (i < found.size()); ++i) {
        isUnique = (found[i] == this);
    }
    if (!isUnique) {
        QString msg = "Variable '%1' is not unique in '%2'";
        throw Exception(msg.arg(id().label()).arg(par->fullName()));
    }
}

} //namespace


