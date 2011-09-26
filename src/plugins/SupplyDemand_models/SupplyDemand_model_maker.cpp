/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "SupplyDemand_model_maker.h"
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
#include "weather.h"

using namespace UniSim;

namespace SupplyDemand{

QString SupplyDemandModelMaker::pluginDesc() const
{
	return "Description of SupplyDemand";
}

Identifier SupplyDemandModelMaker::pluginName() const {
	return "SupplyDemand";
}

QStringList SupplyDemandModelMaker::authors() const
{
	return QStringList() << "Niels";
}

const QMap<Identifier, QString>& SupplyDemandModelMaker::supportedClasses()
{
	if (!desc.isEmpty()) return desc;

    desc["Acquisition"] =
	"Description";

    desc["EnergyBudget"] =
    "Description";

    desc["GrowthDemand"] =
    "Description";

    desc["Holometabola"] =
	"Description";

    desc["LifeTable"] =
    "Description";

    desc["Maize"] =
    "Description";

    desc["ReproductionDemandPerCapita"] =
    "Description";

    desc["ReproductionDemandSizeDependent"] =
    "Description";

    desc["RespirationDemand"] =
    "Description";

    desc["SplitOnShorteningDays"] =
	"Description";

    desc["LifeStage"] =
    "Description";

    desc["Weather"] =
    "Description";

    desc["Exposure"] =
    "Description";

    return desc;
}

#define UNISIM_CREATE(Name) if (modelType.equals(#Name)) model = new Name(objectName, parent)

Model* SupplyDemandModelMaker::create(Identifier modelType, Identifier objectName, QObject *parent)
{
	setSimulationObjectFromDescendent(parent);
	Model *model = 0;
    UNISIM_CREATE(Acquisition);
    UNISIM_CREATE(EnergyBudget);
    UNISIM_CREATE(Exposure);
    UNISIM_CREATE(GrowthDemand);
    UNISIM_CREATE(Holometabola);
    UNISIM_CREATE(LifeTable);
    UNISIM_CREATE(LifeStage);
    UNISIM_CREATE(Maize);
    UNISIM_CREATE(ReproductionDemandPerCapita);
    UNISIM_CREATE(ReproductionDemandSizeDependent);
    UNISIM_CREATE(RespirationDemand);
    UNISIM_CREATE(SplitOnShorteningDays);
    UNISIM_CREATE(Weather);
	return model;
}


Q_EXPORT_PLUGIN2(SupplyDemand_model_maker, SupplyDemandModelMaker)

} //namespace
