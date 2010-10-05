/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "seed_emergence_model_maker.h"
#include "seed_bank.h"
#include "weather.h"

using namespace UniSim;

namespace seed_emergence{

QString Seed_emergenceModelMaker::pluginDesc() const
{
	return "Description of seed_emergence";
}

Identifier Seed_emergenceModelMaker::pluginName() const {
	return "seed_emergence";
}

QStringList Seed_emergenceModelMaker::authors() const
{
	return QStringList() << "author1" << "author2";
}

const QMap<Identifier, QString>& Seed_emergenceModelMaker::supportedClasses()
{
	if (!desc.isEmpty()) return desc;

	desc["SeedBank"] =
	"Description";

	desc["Weather"] =
	"Description";

	return desc;
}

void Seed_emergenceModelMaker::useObjectPool(ObjectPool *pool) const {
	objectPool()->deferTo(pool);
}
Model* Seed_emergenceModelMaker::create(Identifier modelType, Identifier objectName, QObject *parent)
{
	setSimulationObjectFromDescendent(parent);
	Model *model = 0;
	if (modelType.equals("seedbank"))
		model = new SeedBank(objectName, parent);
	else if (modelType.equals("weather"))
		model = new Weather(objectName, parent);
	return model;
}


Q_EXPORT_PLUGIN2(seed_emergence_model_maker, Seed_emergenceModelMaker)

} //namespace
