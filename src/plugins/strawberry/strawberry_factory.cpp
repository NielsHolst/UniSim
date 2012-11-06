/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "infection.h"
#include "leaf_production.h"
#include "strawberry_factory.h"

using namespace UniSim;

namespace strawberry{

void StrawberryFactory::defineProducts() {
    addProduct<Infection>("Infection", this, "Computes daily infection rate of leaves");
    addProduct<LeafProduction>("LeafProduction", this, "Computes daily production of new leaves");
}

UniSim::Identifier StrawberryFactory::id() const {
    return "strawberry";
}

QString StrawberryFactory::description() const {
    return
    "The strawberry plugin models powdery mildew in strawberries";
}

QStringList StrawberryFactory::authors() const {
    return QStringList()
        << "Niels";
    // Add Belachew here
}

QObject* StrawberryFactory::asQObject() {
   return this;
}

Q_EXPORT_PLUGIN2(strawberry_factory, StrawberryFactory)

} //namespace
