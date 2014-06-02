/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "publish.h"
#include "test_factory.h"

using namespace UniSim;

namespace test{

// "The @F test plugin contains models used for testing only (UniSim unit tests).";

Identifier TestFactory::id() const {
    return "test";
}

QList<Identifier> TestFactory::inventory() {
    return productList().keys();
}

QObject* TestFactory::create(Identifier className, Identifier objectName, QObject *parent) {
    ProductList::const_iterator p = productList().find(className);
    return (p == productList().end())
            ? 0
            : p.value()->create(objectName, parent);
}

} //namespace
