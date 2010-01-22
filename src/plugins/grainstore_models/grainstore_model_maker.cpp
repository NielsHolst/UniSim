/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "grainstore_model_maker.h"
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "larva_time.h"
#include "maize.h"
#include "prostephanus.h"
#include "pt_development.h"
#include "pt_emigration.h"
#include "pt_immigration.h"
#include "pt_reproduction.h"
#include "weather.h"

using namespace UniSim;

namespace grainstore{

QList<Identifier> GrainstoreModelMaker::modelTypes() const
{
	return QList<Identifier>()
        << Identifier("LarvaTime")
        << Identifier("Maize")
		<< Identifier("Prostephanus")
		<< Identifier("PtDevelopment")
		<< Identifier("PtEmigration")
        << Identifier("PtImmigration")
        << Identifier("PtReproduction")
        << Identifier("Weather")
;
}

Identifier GrainstoreModelMaker::plugInName() const {
    return Identifier("grainstore");
}

void GrainstoreModelMaker::useObjectPool(ObjectPool *pool) const {
	objectPool()->deferTo(pool);
}

Model* GrainstoreModelMaker::create(Identifier modelType, Identifier objectName, QObject *parent)
{
	setSimulationObjectFromDescendent(parent);
	Model *model = 0;
    if (modelType.equals("LarvaTime"))
        model = new LarvaTime(objectName, parent);
    else if (modelType.equals("Maize"))
		model = new Maize(objectName, parent);
    else if (modelType.equals("Prostephanus"))
		model = new Prostephanus(objectName, parent);
    else if (modelType.equals("PtDevelopment"))
		model = new PtDevelopment(objectName, parent);
    else if (modelType.equals("PtEmigration"))
		model = new PtEmigration(objectName, parent);
    else if (modelType.equals("PtImmigration"))
        model = new PtImmigration(objectName, parent);
    else if (modelType.equals("PtReproduction"))
		model = new PtReproduction(objectName, parent);
    else if (modelType.equals("Weather"))
        model = new Weather(objectName, parent);
return model;
}

Q_EXPORT_PLUGIN2(grainstore_model_maker, GrainstoreModelMaker)

} //namespace
