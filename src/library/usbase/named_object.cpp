/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "named_object.h"
#include "utilities.h"

namespace UniSim{

NamedObject::NamedObject(Identifier name, QObject *parent)
    : QObject(parent), _id(name)
{
    setObjectName(name.key());
}
Identifier NamedObject::id() const {
    return _id;
}

QString NamedObject::fullName() const {
    return UniSim::fullName(this);
}
} //namespace

