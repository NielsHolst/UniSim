/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "strawberry_factory.h"
#include "even_odd.h"

using namespace UniSim;

namespace strawberry{

void StrawberryFactory::defineProducts() {
    // Add you own models here...
    addProduct<EvenOdd>("EvenOdd", this, "Even-and-odd dynamics model");
}

UniSim::Identifier StrawberryFactory::id() const {
    return "strawberry";
}

QString StrawberryFactory::description() const {
    return
    "The strawberry plugin includes models for creatures of the strawberry";
}

QStringList StrawberryFactory::authors() const {
    return QStringList()
        << "Niels";
}

QObject* StrawberryFactory::asQObject() {
   return this;
}

Q_EXPORT_PLUGIN2(strawberry_factory, StrawberryFactory)

} //namespace
