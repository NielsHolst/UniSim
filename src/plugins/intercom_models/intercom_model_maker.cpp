/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "area.h"
#include "assimilation_max_given_temp.h"
#include "assimilation_max_given_temp_sla.h"
#include "community.h"
#include "organ.h"
#include "height.h"
#include "intercom_model_maker.h"
#include "leaf_stem_ratio.h"
#include "plant.h"
#include "specific_leaf_area.h"
#include "weather.h"

using namespace UniSim;

namespace intercom{

bool initialized = false;

QList<Identifier> IntercomModelMaker::modelTypes() const
{
	return QList<Identifier>()
        << Identifier("Area")
        << Identifier("AssimilationMaxGivenTemp")
        << Identifier("AssimilationMaxGivenTempSla")
        << Identifier("Community")
        << Identifier("Height")
        << Identifier("LeafStemRatio")
        << Identifier("Organ")
        << Identifier("Plant")
        << Identifier("SpecificLeafArea")
        << Identifier("Weather")
;
}

UniSim::Identifier IntercomModelMaker::plugInName() const {
    return "intercom";
}


void IntercomModelMaker::useObjectPool(ObjectPool *pool) const {
    objectPool()->deferTo(pool);
}

Model* IntercomModelMaker::create(Identifier modelType, Identifier objectName, QObject *parent)
{
	setSimulationObjectFromDescendent(parent);
	Model *model = 0;
    if (modelType.equals("Area"))
        model = new Area(objectName, parent);
    else if (modelType.equals("AssimilationMaxGivenTemp"))
        model = new AssimilationMaxGivenTemp(objectName, parent);
    else if (modelType.equals("AssimilationMaxGivenTempSla"))
        model = new AssimilationMaxGivenTempSla(objectName, parent);
    else if (modelType.equals("Community"))
        model = new Community(objectName, parent);
    else if (modelType.equals("Height"))
        model = new Height(objectName, parent);
    else if (modelType.equals("LeafStemRatio"))
        model = new LeafStemRatio(objectName, parent);
    else if (modelType.equals("Organ"))
        model = new Organ(objectName, parent);
    else if (modelType.equals("Plant"))
		model = new Plant(objectName, parent);
    else if (modelType.equals("SpecificLeafArea"))
        model = new SpecificLeafArea(objectName, parent);
    else if (modelType.equals("Weather"))
		model = new Weather(objectName, parent);
	return model;
}

Q_EXPORT_PLUGIN2(Intercom_model_maker, IntercomModelMaker)

} //namespace
