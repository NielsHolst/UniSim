/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "ocean_factory.h"
#include "even_odd.h"

using namespace UniSim;

namespace ocean{

void OceanFactory::defineProducts() {
    // Add you own models here...
    addProduct<EvenOdd>("EvenOdd", this, "Even-and-odd dynamics model");
}

UniSim::Identifier OceanFactory::id() const {
    return "ocean";
}

QString OceanFactory::description() const {
    return
    "The ocean plugin includes models for creatures of the ocean";
}

QStringList OceanFactory::authors() const {
    return QStringList()
        << "Niels";
}

QObject* OceanFactory::asQObject() {
   return this;
}

#if QT_VERSION < 0x50000
Q_EXPORT_PLUGIN2(ocean_factory, OceanFactory)
#endif

} //namespace
