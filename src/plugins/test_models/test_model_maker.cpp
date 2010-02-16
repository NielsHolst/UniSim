/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "test_model_maker.h"
#include "constant_world.h"
#include "life_cycle.h"
#include "life_stage.h"

using namespace UniSim;

namespace test{

QList<Identifier> TestModelMaker::supportedTypes() const
{
	return QList<Identifier>()
		<< Identifier("ConstantWorld")
		<< Identifier("LifeCycle")
		<< Identifier("LifeStage")
;
}

UniSim::Identifier TestModelMaker::plugInName() const {
    return "test";
}

void TestModelMaker::useObjectPool(ObjectPool *pool) const {
    objectPool()->deferTo(pool);
}

Model* TestModelMaker::create(Identifier modelType, Identifier objectName, QObject *parent)
{
    UniSim::setSimulationObjectFromDescendent(parent);
    Model *model = 0;
	if (modelType.equals("constantworld"))
		model = new ConstantWorld(objectName, parent);
	else if (modelType.equals("lifecycle"))
		model = new LifeCycle(objectName, parent);
	else if (modelType.equals("lifestage"))
		model = new LifeStage(objectName, parent);
	return model;
}

Q_EXPORT_PLUGIN2(test_model_maker, TestModelMaker)

} //namespace
