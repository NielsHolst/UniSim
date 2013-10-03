/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "example_factory.h"
#include "even_odd.h"

using namespace UniSim;

namespace example{

void ExampleFactory::defineProducts() {
    // Add you own models here...
    addProduct<EvenOdd>("EvenOdd", this, "Even-and-odd dynamics model");
}

UniSim::Identifier ExampleFactory::id() const {
    return "example";
}

QString ExampleFactory::description() const {
    return
    "The example plugin includes models for creatures of the example";
}

QStringList ExampleFactory::authors() const {
    return QStringList()
        << "Niels";
}

QObject* ExampleFactory::asQObject() {
   return this;
}

#if QT_VERSION < 0x50000
Q_EXPORT_PLUGIN2(example_factory, ExampleFactory)
#endif

} //namespace
