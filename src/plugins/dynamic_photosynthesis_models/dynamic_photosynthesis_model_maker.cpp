/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "dynamic_photosynthesis_model_maker.h"
#include "leaf.h"

using namespace UniSim;

namespace dynamic_photosynthesis{

QString Dynamic_photosynthesisModelMaker::pluginDesc() const
{
    return "The @F dynamic_photosynthesis plug-in contains just one model @F {Leaf}, "
            "which models photosynthesis based on induction state";
}

Identifier Dynamic_photosynthesisModelMaker::pluginName() const {
	return "dynamic_photosynthesis";
}

QStringList Dynamic_photosynthesisModelMaker::authors() const
{
	return QStringList() << "Isik" << "Niels";
}

const QMap<Identifier, QString>& Dynamic_photosynthesisModelMaker::supportedClasses()
{
	if (!desc.isEmpty()) return desc;

    desc["Leaf"] =
        "To simulate leaf photosynthesis in C3 plants";

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
	return model;
}


Q_EXPORT_PLUGIN2(dynamic_photosynthesis_model_maker, Dynamic_photosynthesisModelMaker)

} //namespace
