/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "aphid_factory.h"
#include "aphid_fecundity.h"
#include "damagethreshold.h"

using namespace UniSim;

namespace aphid{

void AphidFactory::defineProducts() {
    // Add your own models here...
    AddProduct(AphidFecundity, "Aphid fecundity model");
    AddProduct(DamageThreshold, "Damage Threshold model");
}

UniSim::Identifier AphidFactory::id() const {
    return "aphid";
}

QString AphidFactory::description() const {
    return
    "The aphid plugin includes models for teaching purposes";
}

QStringList AphidFactory::authors() const {
    return QStringList()
        << "Niels";
}

QObject* AphidFactory::asQObject() {
   return this;
}

} //namespace
