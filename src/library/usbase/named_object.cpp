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
    return heading + "/" + _id.label();
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

NamedObject::AbsolutePath NamedObject::absolutePath(QString name) {
    if (name[0] != '.') {
        return AbsolutePath(root(), name);
    }

    AbsolutePath res = AbsolutePath(this, name);
    res.origin = this;
    res.absoluteName = name.mid(2);

    if (name.left(2) == "./") {
        if (res.absoluteName[0] == '.') {
            QString msg = "'./' cannot be followed by another indirection (i.e. a '.') in '%1'";
            throw Exception(msg.arg(name), this);
        }
    }
    else {
        while (res.absoluteName.left(3) == "../") {
            res.origin = res.origin->seekParent<NamedObject*>("*");
            res.absoluteName = res.absoluteName.mid(3);
        }
        if (res.absoluteName.left(2) == "./") {
            QString msg = "A '../' cannot be follow by a './' indirection in '%1'";
            throw Exception(msg.arg(name), this);
        }
        if (res.absoluteName[0] == '/') {
            QString msg = "Double-slash '//' not allowed in '%1'";
            throw Exception(msg.arg(name), this);
        }
    }
    return res;
}

} //namespace

