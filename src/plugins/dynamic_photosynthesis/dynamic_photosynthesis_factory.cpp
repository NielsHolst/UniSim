/* Copyright (C) 2013 by Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "publish.h"
#include "dynamic_photosynthesis_factory.h"

using namespace UniSim;

namespace dynamic_photosynthesis {

Identifier DynamicPhotosynthesis::id() const {
    return "dynamic_photosynthesis";
}

QList<Identifier> DynamicPhotosynthesis::inventory() {
    return productList().keys();
}

QObject* DynamicPhotosynthesis::create(Identifier className, Identifier objectName, QObject *parent) {
    ProductList::const_iterator p = productList().find(className);
    return (p == productList().end())
            ? 0
            : p.value()->create(objectName, parent);
}

} //namespace
