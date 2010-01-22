/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "beehive_model_maker.h"
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "aethina.h"

using namespace UniSim;

namespace beehive{

QList<Identifier> BeehiveModelMaker::modelTypes() const
{
	return QList<Identifier>()
		<< Identifier("Aethina")
;
}

Identifier BeehiveModelMaker::plugInName() const {
	return Identifier("beehive");
}

void BeehiveModelMaker::useObjectPool(ObjectPool *pool) const {
	objectPool()->deferTo(pool);
}

Model* BeehiveModelMaker::create(Identifier modelType, Identifier objectName, QObject *parent)
{
	setSimulationObjectFromDescendent(parent);
	Model *model = 0;
	if (modelType.equals("aethina"))
		model = new Aethina(objectName, parent);
	return model;
}

Q_EXPORT_PLUGIN2(beehive_model_maker, BeehiveModelMaker)

} //namespace
