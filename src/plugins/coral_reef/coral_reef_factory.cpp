/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "coral_reef_factory.h"
#include "spawning.h"
#include "logistic_growth.h"

using namespace UniSim;

namespace coral_reef{

void Coral_reefFactory::defineProducts() {
    // Add your own models here...
    AddProduct(Spawning, "Model of spawning event");
    AddProduct(LogisticGrowth, "Logistic growth model");
}

UniSim::Identifier Coral_reefFactory::id() const {
    return "coral_reef";
}

QString Coral_reefFactory::description() const {
    return
    "The coral_reef plugin includes models for teaching purposes";
}

QStringList Coral_reefFactory::authors() const {
    return QStringList()
        << "Niels";
}

QObject* Coral_reefFactory::asQObject() {
   return this;
}

} //namespace
