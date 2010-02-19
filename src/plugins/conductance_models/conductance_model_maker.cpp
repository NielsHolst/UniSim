/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "conductance_model_maker.h"
#include "community.h"
#include "plant.h"
#include "weather.h"

using namespace UniSim;

namespace conductance{

QList<Identifier> ConductanceModelMaker::supportedTypes() const
{
	return QList<Identifier>()
        << Identifier("Community")
        << Identifier("Plant")
		<< Identifier("Weather")
;
}

Identifier ConductanceModelMaker::plugInName() const {
	return Identifier("conductance");
}

void ConductanceModelMaker::useObjectPool(ObjectPool *pool) const {
	objectPool()->deferTo(pool);
}

Model* ConductanceModelMaker::create(Identifier modelType, Identifier objectName, QObject *parent)
{
	setSimulationObjectFromDescendent(parent);
	Model *model = 0;
    if (modelType.equals("community"))
        model = new Community(objectName, parent);
    else if (modelType.equals("plant"))
		model = new Plant(objectName, parent);
	else if (modelType.equals("weather"))
		model = new Weather(objectName, parent);
	return model;
}

Q_EXPORT_PLUGIN2(conductance_model_maker, ConductanceModelMaker)

} //namespace
