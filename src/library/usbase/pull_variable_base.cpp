/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#include "pull_variable_base.h"

namespace UniSim{


PullVariableBase::PullVariableBase(Identifier id, QObject *parent, QString description)
    : QObject(parent), _id(id), _description(description)
{
    setObjectName(id.key());
}

Identifier PullVariableBase::id() const {
    return _id;
}

QString PullVariableBase::description() const {
    return _description;
}


} //namespace


