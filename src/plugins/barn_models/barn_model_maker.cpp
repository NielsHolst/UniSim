/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "barn_model_maker.h"
#include "predator.h"
#include "prey.h"

using namespace UniSim;

namespace barn{

QString BarnModelMaker::pluginDesc() const
{
	return "Description of barn";
}

Identifier BarnModelMaker::pluginName() const {
	return "barn";
}

QStringList BarnModelMaker::authors() const
{
	return QStringList() << "author1" << "author2";
}

const QMap<Identifier, QString>& BarnModelMaker::supportedClasses()
{
	if (!desc.isEmpty()) return desc;

	desc["Predator"] =
	"Description";

	desc["Prey"] =
	"Description";

	return desc;
}

void BarnModelMaker::useObjectPool(ObjectPool *pool) const {
	objectPool()->deferTo(pool);
}
Model* BarnModelMaker::create(Identifier modelType, Identifier objectName, QObject *parent)
{
	setSimulationObjectFromDescendent(parent);
	Model *model = 0;
	if (modelType.equals("predator"))
		model = new Predator(objectName, parent);
	else if (modelType.equals("prey"))
		model = new Prey(objectName, parent);
	return model;
}


Q_EXPORT_PLUGIN2(barn_model_maker, BarnModelMaker)

} //namespace
