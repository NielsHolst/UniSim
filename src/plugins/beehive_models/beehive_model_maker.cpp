/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "beehive_model_maker.h"
#include "fecundity.h"
#include "growth.h"
#include "life_stage.h"
#include "resource.h"
#include "small_hive_beetle.h"

using namespace UniSim;

namespace beehive{

QString BeehiveModelMaker::pluginDesc() const
{
	return "Description of beehive";
}

Identifier BeehiveModelMaker::pluginName() const {
	return "beehive";
}

QStringList BeehiveModelMaker::authors() const
{
    return QStringList() << "William" << "Niels";
}

const QMap<Identifier, QString>& BeehiveModelMaker::supportedClasses()
{
	if (!desc.isEmpty()) return desc;

    desc["Fecundity"] =
    "Description";

    desc["Growth"] =
    "Description";

    desc["LifeStage"] =
	"Description";

	desc["Resource"] =
	"Description";

    desc["SmallHiveBeetle"] =
    "Description";

	return desc;
}

Model* BeehiveModelMaker::create(Identifier modelType, Identifier objectName, QObject *parent)
{
	setSimulationObjectFromDescendent(parent);
	Model *model = 0;
    if (modelType.equals("Fecundity"))
        model = new Fecundity(objectName, parent);
    else if (modelType.equals("Growth"))
        model = new Growth(objectName, parent);
    else if (modelType.equals("LifeStage"))
		model = new LifeStage(objectName, parent);
    else if (modelType.equals("Resource"))
		model = new Resource(objectName, parent);
    else if (modelType.equals("SmallHiveBeetle"))
        model = new SmallHiveBeetle(objectName, parent);
    return model;
}


Q_EXPORT_PLUGIN2(beehive_model_maker, BeehiveModelMaker)

} //namespace
