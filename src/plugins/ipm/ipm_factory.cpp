/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "ipm_factory.h"
#include "even_odd.h"

using namespace UniSim;

namespace ipm{

void IpmFactory::defineProducts() {
    // Add your own models here...
    AddProduct(EvenOdd, "Even-and-odd dynamics model");
}

UniSim::Identifier IpmFactory::id() const {
    return "ipm";
}

QString IpmFactory::description() const {
    return
    "The ipm plugin includes models for teaching purposes";
}

QStringList IpmFactory::authors() const {
    return QStringList()
        << "Niels";
}

QObject* IpmFactory::asQObject() {
   return this;
}

#if QT_VERSION < 0x50000
Q_EXPORT_PLUGIN2(ipm_factory, IpmFactory)
#endif

} //namespace
