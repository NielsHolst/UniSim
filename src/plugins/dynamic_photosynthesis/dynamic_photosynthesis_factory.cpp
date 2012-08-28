/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "dynamic_photosynthesis_factory.h"
#include "leaf.h"

using namespace UniSim;

namespace dynamic_photosynthesis{

void DynamicPhotosynthesisFactory::defineProducts() {
    addProduct<Leaf>("Leaf", this, "To simulate leaf photosynthesis in C3 plants");
}

UniSim::Identifier DynamicPhotosynthesisFactory::id() const {
    return "dynamic_photosynthesis";
}

QString DynamicPhotosynthesisFactory::description() const {
    return
    "The @F dynamic_photosynthesis plug-in contains just one model @F {Leaf}, "
    "which models photosynthesis based on induction state";
}

QStringList DynamicPhotosynthesisFactory::authors() const {
    return QStringList() << "Isik" << "Niels";
}

QObject* DynamicPhotosynthesisFactory::asQObject() {
   return this;
}

Q_EXPORT_PLUGIN2(dynamic_photosynthesis_factory, DynamicPhotosynthesisFactory)

} //namespace
