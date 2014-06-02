/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "ecotox_factory.h"
#include "publish.h"

using namespace UniSim;

namespace ecotox{

Identifier EcotoxFactory::id() const {
    return "ecotox";
}

QList<Identifier> EcotoxFactory::inventory() {
    return productList().keys();
}

QObject* EcotoxFactory::create(Identifier className, Identifier objectName, QObject *parent) {
    ProductList::const_iterator p = productList().find(className);
    return (p == productList().end())
            ? 0
            : p.value()->create(objectName, parent);
}

} //namespace
