/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "supply_demand_factory.h"
#include "acquisition.h"
#include "energy_budget.h"
#include "exposure.h"
#include "growth_demand.h"
#include "holometabola.h"
#include "life_table.h"
#include "maize.h"
#include "reproduction_demand_per_capita.h"
#include "reproduction_demand_size_dependent.h"
#include "respiration_demand.h"
#include "split_on_shortening_days.h"
#include "life_stage.h"

using namespace UniSim;

namespace SupplyDemand{

void SupplyDemandFactory::defineProducts() {
    addProduct<Acquisition>("Acquisition", this, "Description pending");
    addProduct<EnergyBudget>("EnergyBudget", this, "Description pending");
    addProduct<Exposure>("Exposure", this, "Description pending");
    addProduct<GrowthDemand>("GrowthDemand", this, "Description pending");
    addProduct<Holometabola>("Holometabola", this, "Description pending");
    addProduct<LifeTable>("LifeTable", this, "Description pending");
    addProduct<LifeStage>("LifeStage", this, "Description pending");
    addProduct<Maize>("Maize", this, "Description pending");
    addProduct<ReproductionDemandPerCapita>("ReproductionDemandPerCapita", this, "Description pending");
    addProduct<ReproductionDemandSizeDependent>("ReproductionDemandSizeDependent", this, "Description pending");
    addProduct<RespirationDemand>("RespirationDemand", this, "Description pending");
    addProduct<SplitOnShorteningDays>("SplitOnShorteningDays", this, "Description pending");
}

UniSim::Identifier SupplyDemandFactory::id() const {
    return "SupplyDemand";
}

QString SupplyDemandFactory::description() const {
    return
    "!Exclude\n"
    "Under development";
}

QStringList SupplyDemandFactory::authors() const {
    return QStringList()
        << "Niels";
}

QObject* SupplyDemandFactory::asQObject() {
   return this;
}

#if QT_VERSION < 0x50000
Q_EXPORT_PLUGIN2(supply_demand_factory, SupplyDemandFactory)
#endif

} //namespace
