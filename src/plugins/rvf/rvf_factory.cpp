/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "rvf_factory.h"
#include "publish.h"

using namespace UniSim;

namespace rvf {

Identifier RvfFactory::id() const {
    return "rvf";
}

QList<Identifier> RvfFactory::inventory() {
    return productList().keys();
}

QObject* RvfFactory::create(Identifier className, Identifier objectName, QObject *parent) {
    ProductList::const_iterator p = productList().find(className);
    return (p == productList().end())
            ? 0
            : p.value()->create(objectName, parent);
}

} //namespace
