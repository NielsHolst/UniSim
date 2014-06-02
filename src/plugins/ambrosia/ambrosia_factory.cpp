/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "ambrosia_factory.h"
#include "publish.h"

using namespace UniSim;

namespace ambrosia{
//    "The @F ambrosia plugin is an implementation of the phenology model for @I {Ambrosia artemisiifolia}, "
//    "as described by @Cite{$label{Deen et al. 1998a}deen_et_al_1998a} and @Cite{$label{1998b}deen_et_al_1998b}";

Identifier AmbrosiaFactory::id() const {
    return "ambrosia";
}

QList<Identifier> AmbrosiaFactory::inventory() {
    return productList().keys();
}

QObject* AmbrosiaFactory::create(Identifier className, Identifier objectName, QObject *parent) {
    ProductList::const_iterator p = productList().find(className);
    return (p == productList().end())
            ? 0
            : p.value()->create(objectName, parent);
}

} //namespace
