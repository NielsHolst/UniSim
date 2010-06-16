/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "crop.h"
#include "ess2009_model_maker.h"
#include "instant_mortality.h"
#include "plant_growth_stage.h"
#include "rotation.h"
#include "seed_bank.h"
#include "treatments.h"
#include "weather.h"
#include "weed.h"

using namespace UniSim;

namespace ess2009 {

Identifier Ess2009ModelMaker::pluginName() const
{
    return Identifier("ess2009");
}

QString Ess2009ModelMaker::pluginDesc() const {
    return
    "The @F ess2009 plugin was developed for the ENDURE Summer School 2009 in Volterra, Italy."
    "It contains models to simulate the population dynamics of annual weeds in the crop rotation."
    "The models remain to be formally published.";
}

QStringList Ess2009ModelMaker::authors() const {
    return QStringList()
        << "Niels Holst, Aarhus University, Denmark";
}

const QMap<Identifier, QString>& Ess2009ModelMaker::supportedClasses() {
    if (!desc.isEmpty())
        return desc;

    desc["Crop"] =
    "@I pending";

    desc["InstantMortality"] =
    "@I pending";

    desc["PlantGrowthStage"] =
    "@I pending";

    desc["Rotation"] =
    "@I pending";

    desc["SeedBank"] =
    "@I pending";

    desc["Treatments"] =
    "@I pending";

    desc["Weather"] =
    "@I pending";

    desc["Weed"] =
    "@I pending";

    return desc;
}

void Ess2009ModelMaker::useObjectPool(ObjectPool *pool) const {
    objectPool()->deferTo(pool);
}

Model* Ess2009ModelMaker::create(Identifier modelType, Identifier objectName, QObject *parent)
{
	// Remember to add modelType to the list above as well
    UniSim::setSimulationObjectFromDescendent(parent);
    Model *model=0;
    if (modelType.equals("crop"))
        model = new Crop(objectName, parent);
    else if (modelType.equals("instantmortality"))
        model = new InstantMortality(objectName, parent);
    else if (modelType.equals("plantgrowthstage"))
		model = new PlantGrowthStage(objectName, parent);
    else if (modelType.equals("rotation"))
        model = new Rotation(objectName, parent);
    else if (modelType.equals("seedbank"))
        model = new SeedBank(objectName, parent);
    else if (modelType.equals("treatments"))
        model = new Treatments(objectName, parent);
    else if (modelType.equals("weather"))
        model = new Weather(objectName, parent);
    else if (modelType.equals("weed"))
        model = new Weed(objectName, parent);
    return model;
}

Q_EXPORT_PLUGIN2(ess2009_model_maker,Ess2009ModelMaker)

} //namespace
