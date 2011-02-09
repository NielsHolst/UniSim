/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "dynamic_photosynthesis_model_maker.h"
#include "leaf.h"
#include "weather.h"

using namespace UniSim;

namespace dynamic_photosynthesis{

QString Dynamic_photosynthesisModelMaker::pluginDesc() const
{
	return "Description of dynamic_photosynthesis";
}

Identifier Dynamic_photosynthesisModelMaker::pluginName() const {
	return "dynamic_photosynthesis";
}

QStringList Dynamic_photosynthesisModelMaker::authors() const
{
	return QStringList() << "author1" << "author2";
}

const QMap<Identifier, QString>& Dynamic_photosynthesisModelMaker::supportedClasses()
{
	if (!desc.isEmpty()) return desc;

        desc["Leaf"] =
	"Description";

	desc["Weather"] =
	"Description";

	return desc;
}

void Dynamic_photosynthesisModelMaker::useObjectPool(ObjectPool *pool) const {
	objectPool()->deferTo(pool);
}
Model* Dynamic_photosynthesisModelMaker::create(Identifier modelType, Identifier objectName, QObject *parent)
{
	setSimulationObjectFromDescendent(parent);
	Model *model = 0;
        if (modelType.equals("leaf"))
                model = new Leaf(objectName, parent);
	else if (modelType.equals("weather"))
		model = new Weather(objectName, parent);
	return model;
}


Q_EXPORT_PLUGIN2(dynamic_photosynthesis_model_maker, Dynamic_photosynthesisModelMaker)

} //namespace
