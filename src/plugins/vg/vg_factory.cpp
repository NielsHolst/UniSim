/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "environment.h"
#include "vg_factory.h"

using namespace UniSim;

namespace vg{

void VgFactory::defineProducts() {
    addProduct<Environment>("Environment", this, "desc");
}

UniSim::Identifier VgFactory::id() const {
    return "vg";
}

QString VgFactory::description() const {
    return
    "Virtual greenhouse";
}

QStringList VgFactory::authors() const {
    return QStringList()
            << "Oliver"
            << "Niels";
}

QObject* VgFactory::asQObject() {
   return this;
}

#if QT_VERSION < 0x50000
Q_EXPORT_PLUGIN2(vg_factory, VgFactory)
#endif

} //namespace
