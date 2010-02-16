/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "ambrosia_model_maker.h"
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "plant.h"
#include "time.h"
#include "weather.h"

using namespace UniSim;

namespace ambrosia{

QList<Identifier> AmbrosiaModelMaker::supportedTypes() const
{
	return QList<Identifier>()
        << Identifier("Plant")
        << Identifier("Time")
        << Identifier("Weather")
;
}

Identifier AmbrosiaModelMaker::plugInName() const {
	return Identifier("ambrosia");
}

void AmbrosiaModelMaker::useObjectPool(ObjectPool *pool) const {
	objectPool()->deferTo(pool);
}

Model* AmbrosiaModelMaker::create(Identifier modelType, Identifier objectName, QObject *parent)
{
	setSimulationObjectFromDescendent(parent);
	Model *model = 0;
	if (modelType.equals("time"))
		model = new Time(objectName, parent);
    else if (modelType.equals("Plant"))
        model = new Plant(objectName, parent);
    else if (modelType.equals("Weather"))
        model = new Weather(objectName, parent);
    return model;
}

Q_EXPORT_PLUGIN2(ambrosia_model_maker, AmbrosiaModelMaker)

} //namespace
