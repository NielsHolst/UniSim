/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "insecticide_factory.h"
#include "application.h"
#include "compartment.h"
#include "substance.h"

using namespace UniSim;

namespace insecticide{

void InsecticideFactory::defineProducts() {
    // Add your own models here...
    AddProduct(Application, "An insecticide application");
    AddProduct(Compartment, "A compartpent where insecticide is degrading");
    AddProduct(Substance, "A substance such as parent or metabolite");
}

UniSim::Identifier InsecticideFactory::id() const {
    return "insecticide";
}

QString InsecticideFactory::description() const {
    return
    "The insecticide plugin includes models for insecticide ecotoxicology";
}

QStringList InsecticideFactory::authors() const {
    return QStringList()
        << "Niels";
}

QObject* InsecticideFactory::asQObject() {
   return this;
}

#if QT_VERSION < 0x50000
Q_EXPORT_PLUGIN2(insecticide_factory, InsecticideFactory)
#endif

} //namespace
