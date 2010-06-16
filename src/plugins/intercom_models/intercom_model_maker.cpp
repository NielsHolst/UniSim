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
#include "light_use_efficiency_given_temp.h"
#include "plant.h"
#include "specific_leaf_area.h"
#include "weather.h"

using namespace UniSim;

namespace intercom{

bool initialized = false;

UniSim::Identifier IntercomModelMaker::pluginName() const {
    return "intercom";
}

QString IntercomModelMaker::pluginDesc() const {
    return
    "The @F intercom plugin is an implementation of the INTERCOM model for plant growth and competition, "
    "as described by @Cite{$kropff_laar}.";
}

QStringList IntercomModelMaker::authors() const {
    return QStringList()
        << "Niels Holst, Aarhus University, Denmark"
        << "Marleen Riemens, Plant Research International, Netherlands"
        << "Jonathan Storkey, Rothamstead Research International, UK";
}

const QMap<Identifier, QString>& IntercomModelMaker::supportedClasses() {
    if (!desc.isEmpty())
        return desc;

    desc["Area"] =
    "@I pending";

    desc["AssimilationMaxGivenTemp"] =
    "@I pending";

    desc["AssimilationMaxGivenTempSla"] =
    "@I pending";

    desc["Community"] =
    "@I pending";

    desc["Height"] =
    "@I pending";

    desc["LeafStemRatio"] =
    "@I pending";

    desc["LightUseEfficiencyGivenTemp"] =
    "@I pending";

    desc["Organ"] =
    "@I pending";

    desc["Plant"] =
    "@I pending";

    desc["SpecificLeafArea"] =
    "@I pending";

    desc["Weather"] =
    "@I pending";

    return desc;
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
    else if (modelType.equals("LightUseEfficiencyGivenTemp"))
        model = new LightUseEfficiencyGivenTemp(objectName, parent);
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
