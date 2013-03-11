/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "named_object.h"
#include "utilities.h"

namespace UniSim{

NamedObject:: NamedObject()
    : QObject()
{
}

NamedObject::NamedObject(Identifier name, QObject *parent)
    : QObject(parent), _id(name)
{
    setObjectName(name.key());
}

void NamedObject::setId(Identifier id) {
    _id = id;
}

Identifier NamedObject::id() const {
    return _id;
}

QString NamedObject::fullName() const {
    return UniSim::fullName(this);
}

QString NamedObject::fullLabel() const {
    QString heading;
    NamedObject *p = const_cast<NamedObject*>(this);
    NamedObject *parent = p->peekParent<NamedObject*>("*");
    if (parent)
        heading = parent->fullLabel();
    return heading + "/" + _id.label();
}

//! Finds the root of NamedObject's ancestry
NamedObject* NamedObject::root() {
    NamedObject *root = this;
    NamedObject *parent = peekParent<NamedObject*>("*");
    while (parent) {
        root = parent;
        parent = root->peekParent<NamedObject*>("*");
    }
    return root;

}


} //namespace

