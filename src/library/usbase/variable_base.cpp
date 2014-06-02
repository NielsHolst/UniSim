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

VariableBase::VariableBase(Identifier id, NamedObject *parent)
    : QObject(parent), _id(id), _parent(parent)
{
    setObjectName(id.key());
    assertUniqueness();
}

void VariableBase::assertUniqueness() {
    auto meAndSiblings = _parent->seekChildren<VariableBase*>(id().key());
    if (meAndSiblings.size() != 1) {
        QString msg = "Variable '%1' is not unique in '%2'";
        throw Exception(msg.arg(_id.label()).arg(_parent->fullLabel()));
    }
//    QList<VariableBase*> found = _parent->seekChildren<VariableBase*>(id().key());
//    bool isUnique = true;
//    for (int i=0; isUnique && (i < found.size()); ++i) {
//        isUnique = (found[i] == this);
//    }
//    if (!isUnique) {
//        QString msg = "Variable '%1' is not unique in '%2'";
//        throw Exception(msg.arg(id().label()).arg(parent->fullName()));
//    }
}

} //namespace


