/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#include "factory_plug_in.h"
#include "product_base.h"

namespace UniSim{

ProductBase::ProductBase(Identifier id, FactoryPlugIn *parent, QString description)
    : QObject(parent->asQObject()),
      _id(id),
      _factory(parent),
      _description(description)
{
    setObjectName(id.key());
}

Identifier ProductBase::id() const {
    return _id;
}

FactoryPlugIn* ProductBase::factory() const {
    return _factory;
}

QString ProductBase::description() const {
    return _description;
}

} //namespace
