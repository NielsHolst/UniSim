/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "separation_model_maker.h"
#include "liquid.h"
#include "separator.h"
#include "solid.h"

using namespace UniSim;

namespace separation{

QString SeparationModelMaker::pluginDesc() const
{
	return "Description of separation";
}

Identifier SeparationModelMaker::pluginName() const {
	return "separation";
}

QStringList SeparationModelMaker::authors() const
{
	return QStringList() << "author1" << "author2";
}

const QMap<Identifier, QString>& SeparationModelMaker::supportedClasses()
{
	if (!desc.isEmpty()) return desc;

	desc["Liquid"] =
	"Description";

	desc["Separator"] =
	"Description";

	desc["Solid"] =
	"Description";

	return desc;
}

Model* SeparationModelMaker::create(Identifier modelType, Identifier objectName, QObject *parent)
{
	setSimulationObjectFromDescendent(parent);
	Model *model = 0;
	if (modelType.equals("liquid"))
		model = new Liquid(objectName, parent);
	else if (modelType.equals("separator"))
		model = new Separator(objectName, parent);
	else if (modelType.equals("solid"))
		model = new Solid(objectName, parent);
	return model;
}


Q_EXPORT_PLUGIN2(separation_model_maker, SeparationModelMaker)

} //namespace
