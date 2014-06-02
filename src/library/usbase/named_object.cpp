/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "named_object.h"
#include "utilities.h"

namespace UniSim{

long int NamedObject::objectCount = 0;

NamedObject::NamedObject()
    : QObject()
{
    myNumber = objectCount++;
}

NamedObject::NamedObject(Identifier name, QObject *parent)
    : QObject(parent), _id(name)
{
    setObjectName(name.key());
    myNumber = objectCount++;
}

void NamedObject::setId(Identifier id) {
    _id = id;
}

Identifier NamedObject::id() const {
    return _id;
}

QString NamedObject::uniqueId() const {
    return "x" + _id.key() + QString::number(number());
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
    QString myId = _id.label();
    if (myId.isEmpty()) myId = "*";
    return heading + "/" + myId;
}

long int NamedObject::number() const {
    return myNumber;
}

void NamedObject::resetNumbering() {
    objectCount = 0;
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

QString NamedObject::absolutePath(QString path) {
    if (path[0] != '.') return path;
    NamedObject *origin = this;
    QString tail = path;
    while (tail.left(2) == "..") {
        origin = origin->seekParent<NamedObject*>("*");
        tail = tail.mid( tail[2]== '/' ? 3 : 1);
    }
    if (tail.left(2) == "./" || tail.left(2) == ".[")
        tail = tail.mid(1);
    else
        tail.prepend("/");
    return origin->fullLabel() + tail;
}

} //namespace

