/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "crop.h"
#include "awe_model_maker.h"
#include "instant_mortality.h"
#include "plant_growth_stage.h"
#include "rotation.h"
#include "seed_bank.h"
#include "weather.h"
#include "weed.h"

using namespace UniSim;

namespace awe {

Identifier AweModelMaker::pluginName() const
{
    return Identifier("awe");
}

QString AweModelMaker::pluginDesc() const {
    return
    "The @F awe plugin contains an unpublished model of annual weed population dynamics in the crop rotation. "
    " It was developed as a teaching tool for the ENDURE Summer School 2009 in Volterra, Italy.";
}

QStringList AweModelMaker::authors() const {
    return QStringList()
        << "Niels Holst, Aarhus University, Denmark";
}

const QMap<Identifier, QString>& AweModelMaker::supportedClasses() {
    if (!desc.isEmpty())
        return desc;

    desc["Crop"] =
    "The @F Crop model is quite simple as it does not contain a dynamic formulation of growht and development. "
    "Rather, the crop is modelled only by its leaf area index (LAI) which follows a fixed schedule according "
    "to the temperature sum (day-degrees above @Char {ring}C) since sowing.";

    desc["InstantMortality"] =
    "Once per year, at the given month and day, an @F InstantMortality object effectuates "
    "given mortalities on one or more @F PlantGrowhtStage models. This can be used to simulate "
    "discrete mortaly events caused by field operations, for example, spraying or soil "
    "cultivation. @PP It is natural to create @F InstantMortality objects as descendants "
    "(or, 'inside') of a @F Crop model. The mortalities will then only be in effect when that "
    "crop is current in the @F {Rotation}.";

    desc["PlantGrowthStage"] =
    "A @F PlantGrowthStage holds two @F UniSim::Stage models inside, represing plant density "
    "both as plants per m @Sup 2 and density equivalents per m @Sup {2}. One density eqivalent "
    "corresponds to the density of one plant that emerged simultaneouly with the crop. "
    "Later emerging plant counts as less than one density equivalent per plant. Thus "
    "'equivalence'' is meant as equivalent in terms of competitiveness towards the crop";

    desc["Rotation"] =
    "The @F Rotation model holds as children the @F Crop models that are part of the rotation. "
    "It keeps check of the current crop according to cropping sequence "
    "and the harvest and sowing events pertaining to @F Crop models.";

    desc["SeedBank"] =
    "The @F SeedBank model holds the density of seeds in the soil (per @Sup {2}). "
    "Seeds are divide into two groups: dormant and non-dormant. "
    "Newly produced seeds enter the seed bank as dormant. "
    "Dormancy is lost on 1 January every year, and all dormant seeds are then moved"
    "into the non-dormant group.";

    desc["Weather"] =
    "The @F Weather model produces data for daily average temperature, assuming a sine curve"
    "relation over the year, @Math{ y = a + b ({pi x over 365 + c}) sup 2}, "
    "where @F x is the day of the year [0..365].";

    desc["Weed"] =
    "The @F Weed model holds the @F SeedBank model, @F PlantGrowthStage models for "
    "seedling, juvenile and mature stages, and a @F UniSim::Stage model "
    "to represent seeds on the plant. @PP "
    "The @F Weed model orchestrates the progressive flow of weed individuals "
    "through these models. For the @F PlantGrowthStage models there are parallel flows of "
    "density equivalents.";

    return desc;
}

void AweModelMaker::useObjectPool(ObjectPool *pool) const {
    objectPool()->deferTo(pool);
}

Model* AweModelMaker::create(Identifier modelType, Identifier objectName, QObject *parent)
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
    else if (modelType.equals("weather"))
        model = new Weather(objectName, parent);
    else if (modelType.equals("weed"))
        model = new Weed(objectName, parent);
    return model;
}

Q_EXPORT_PLUGIN2(awe_model_maker,AweModelMaker)

} //namespace
