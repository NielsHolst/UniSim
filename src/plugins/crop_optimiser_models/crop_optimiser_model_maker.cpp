/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "crop_optimiser_model_maker.h"
#include "crop.h"
#include "farm.h"
#include "field.h"
#include "landscape.h"
#include "livestock.h"

using namespace UniSim;

namespace crop_optimiser{

QString Crop_optimiserModelMaker::pluginDesc() const
{
	return "Description of crop_optimiser";
}

Identifier Crop_optimiserModelMaker::pluginName() const {
	return "crop_optimiser";
}

QStringList Crop_optimiserModelMaker::authors() const
{
	return QStringList() << "author1" << "author2";
}

const QMap<Identifier, QString>& Crop_optimiserModelMaker::supportedClasses()
{
	if (!desc.isEmpty()) return desc;

	desc["Farm"] =
	"Description";

	desc["Field"] =
	"Description";

        desc["Crop"] =
        "Description";

        desc["Livestock"] =
        "Description";

        desc["Landscape"] =
        "Description";

	return desc;
}

Model* Crop_optimiserModelMaker::create(Identifier modelType, Identifier objectName, QObject *parent)
{
	setSimulationObjectFromDescendent(parent);
	Model *model = 0;
	if (modelType.equals("farm"))
		model = new Farm(objectName, parent);
	else if (modelType.equals("field"))
                model = new Field(objectName, parent);
        else if (modelType.equals("crop"))
                 model = new Crop(objectName, parent);
        else if (modelType.equals("landscape"))
                 model = new Landscape(objectName, parent);
        else if (modelType.equals("livestock"))
                 model = new Livestock(objectName, parent);

	return model;
}


Q_EXPORT_PLUGIN2(crop_optimiser_model_maker, Crop_optimiserModelMaker)

} //namespace
