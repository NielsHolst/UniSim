/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "lakeoxygen_model_maker.h"
#include "lake_environment.h"
#include "net_production.h"
#include "oxygen_concentration.h"

using namespace UniSim;

namespace lakeoxygen{

QString LakeoxygenModelMaker::pluginDesc() const
{
	return "Description of lakeoxygen";
}

Identifier LakeoxygenModelMaker::pluginName() const {
	return "lakeoxygen";
}

QStringList LakeoxygenModelMaker::authors() const
{
    return QStringList()
        << "Anders" << "Niels";
}

const QMap<Identifier, QString>& LakeoxygenModelMaker::supportedClasses()
{
	if (!desc.isEmpty()) return desc;

	desc["LakeEnvironment"] =
	"Description";

    desc["NetProduction"] =
    "Description";

	desc["OxygenConcentration"] =
	"Description";

	return desc;
}

void LakeoxygenModelMaker::useObjectPool(ObjectPool *pool) const {
	objectPool()->deferTo(pool);
}
Model* LakeoxygenModelMaker::create(Identifier modelType, Identifier objectName, QObject *parent)
{
	setSimulationObjectFromDescendent(parent);
	Model *model = 0;
	if (modelType.equals("lakeenvironment"))
		model = new LakeEnvironment(objectName, parent);
    else if (modelType.equals("netproduction"))
        model = new NetProduction(objectName, parent);
	else if (modelType.equals("oxygenconcentration"))
		model = new OxygenConcentration(objectName, parent);
	return model;
}


Q_EXPORT_PLUGIN2(lakeoxygen_model_maker, LakeoxygenModelMaker)

} //namespace
