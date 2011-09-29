/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "ecotox_model_maker.h"
#include "pollen.h"

using namespace UniSim;

namespace ecotox{

QString EcotoxModelMaker::pluginDesc() const
{
	return "Description of ecotox";
}

Identifier EcotoxModelMaker::pluginName() const {
	return "ecotox";
}

QStringList EcotoxModelMaker::authors() const
{
	return QStringList() << "author1" << "author2";
}

const QMap<Identifier, QString>& EcotoxModelMaker::supportedClasses()
{
	if (!desc.isEmpty()) return desc;

	desc["Pollen"] =
	"Description";

	return desc;
}

Model* EcotoxModelMaker::create(Identifier modelType, Identifier objectName, QObject *parent)
{
	setSimulationObjectFromDescendent(parent);
	Model *model = 0;
	if (modelType.equals("pollen"))
		model = new Pollen(objectName, parent);
	return model;
}


Q_EXPORT_PLUGIN2(ecotox_model_maker, EcotoxModelMaker)

} //namespace
