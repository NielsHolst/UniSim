/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "mosquito_fecundity.h"
#include "rvf_factory.h"
#include "water_level.h"
#include "sheep_fecundity.h"
#include "mosquito_infection.h"
#include "sheep_infection.h"

using namespace UniSim;

namespace rvf{

void RvfFactory::defineProducts() {
    // Add you own models here...
    addProduct<MosquitoFecundity>("MosquitoFecundity", this, "Daily egg-laying rate of adult mosquitoes. Depends on water level");
    addProduct<WaterLevel>("WaterLevel", this, "Water level in the environment.");
    addProduct<SheepFecundity>("SheepFecundity", this, "Daily birth rate of adult sheep. Depends on Carrying Capacity K");
    addProduct<MosquitoInfection>("MosquitoInfection", this, "Infection transfer from mosquito to sheep given contact rate");
    addProduct<SheepInfection>("SheepInfection", this, "Infection transfer from sheep to mosquitoes given effective contact rate");
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
        << "Niels";
    // Add Clement here. But first add to src/resources/authors.xml
}

QObject* RvfFactory::asQObject() {
   return this;
}

} //namespace
