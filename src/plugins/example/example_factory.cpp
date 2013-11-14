/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "example_factory.h"
#include "even_odd.h"
#include "logistic_growth.h"

using namespace UniSim;

namespace example{

void ExampleFactory::defineProducts() {
    // Add your own models here...
    AddProduct(EvenOdd, "Even-and-odd dynamics model");
    AddProduct(LogisticGrowth, "Logistic growth model");
}

UniSim::Identifier ExampleFactory::id() const {
    return "example";
}

QString ExampleFactory::description() const {
    return
    "The example plugin includes models for teaching purposes";
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
