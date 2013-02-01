/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "mosquito_fecundity.h"
#include "rvf_factory.h"
#include "water_level.h"
#include "inactiveegg.h"
#include "adult.h"
#include "egg.h"

using namespace UniSim;

namespace rvf{

void RvfFactory::defineProducts() {
    // Add you own models here...
    addProduct<MosquitoFecundity>("MosquitoFecundity", this, "Daily egg-laying rate of adult mosquitoes. Depends on water level");
    addProduct<WaterLevel>("WaterLevel", this, "Water level in the environment.");
    addProduct<InactiveEgg>("InactiveEgg", this, "Inactive eggs laid on the ground.");
    addProduct<Adult>("Adult", this, "Adult mosquitoes laying eggs.");
    addProduct<Egg>("Egg", this, "Mosquitoes eggs in water");
}

UniSim::Identifier RvfFactory::id() const {
    return "rvf";
}

QString RvfFactory::description() const {
    return
    "The rvf plugin includes models for the epidemics of Rift Valley Virus (RVF).";
}

QStringList RvfFactory::authors() const {
    return QStringList()
        << "Clement";
    // Add Clement here. But first add to src/resources/authors.xml
}

QObject* RvfFactory::asQObject() {
   return this;
}

#if QT_VERSION < 0x50000
Q_EXPORT_PLUGIN2(rvf_factory, RvfFactory)
#endif

} //namespace
