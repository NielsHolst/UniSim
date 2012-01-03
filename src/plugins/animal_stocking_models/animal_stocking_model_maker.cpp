/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "animal_stocking_model_maker.h"
#include "environment.h"
#include "grass_land.h"
#include "grass_productivity_cutting_estimate.h"
#include "grass_productivity_remote_estimate.h"
#include "stocking_capacity.h"

using namespace UniSim;

namespace AnimalStocking{

QString AnimalStockingModelMaker::pluginDesc() const
{
	return "Description of AnimalStocking";
}

Identifier AnimalStockingModelMaker::pluginName() const {
	return "AnimalStocking";
}

QStringList AnimalStockingModelMaker::authors() const
{
	return QStringList() << "author1" << "author2";
}

const QMap<Identifier, QString>& AnimalStockingModelMaker::supportedClasses()
{
	if (!desc.isEmpty()) return desc;

	desc["Environment"] =
	"Description";

	desc["GrassLand"] =
	"Description";

    desc["GrassProductivityCuttingEstimate"] =
	"Description";

    desc["GrassProductivityRemoteEstimate"] =
    "Description";

    desc["StockingCapacity"] =
	"Description";

	return desc;
}

Model* AnimalStockingModelMaker::create(Identifier modelType, Identifier objectName, QObject *parent)
{
	setSimulationObjectFromDescendent(parent);
	Model *model = 0;
    if (modelType.equals("Environment"))
		model = new Environment(objectName, parent);
    else if (modelType.equals("GrassLand"))
		model = new GrassLand(objectName, parent);
    else if (modelType.equals("GrassProductivityCuttingEstimate"))
        model = new GrassProductivityCuttingEstimate(objectName, parent);
    else if (modelType.equals("GrassProductivityRemoteEstimate"))
        model = new GrassProductivityRemoteEstimate(objectName, parent);
    else if (modelType.equals("StockingCapacity"))
		model = new StockingCapacity(objectName, parent);
	return model;
}


Q_EXPORT_PLUGIN2(AnimalStocking_model_maker, AnimalStockingModelMaker)

} //namespace
