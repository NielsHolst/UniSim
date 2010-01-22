#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "barn_model_maker.h"
#include "predator.h"
#include "prey.h"

using namespace UniSim;

namespace barn{

QList<Identifier> BarnModelMaker::modelTypes() const
{
	return QList<Identifier>()
		<< Identifier("Predator")
		<< Identifier("Prey")
;
}

Identifier BarnModelMaker::plugInName() const {
	return Identifier("barn");
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
