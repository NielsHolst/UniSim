/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "area.h"
#include "area_density_even.h"
#include "area_density_symmetric.h"
#include "area_density_tapering.h"
#include "area_density_topheavy.h"
#include "assimilation_max_given_temp.h"
#include "community.h"
#include "early_growth.h"
#include "organ.h"
#include "height.h"
#include "instant_mortality.h"
#include "intercom_model_maker.h"
#include "light_use_efficiency_given_temp.h"
#include "mass.h"
#include "partitioning_fixed.h"
#include "partitioning_fixed_by_stage.h"
#include "phenology.h"
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
    "as described by @Cite{$label{Kropff \"&\" Laar (1993)}kropff_laar}.";
}

QStringList IntercomModelMaker::authors() const {
    return QStringList()
        << "Niels" << "Marleen" << "Jon";
}

const QMap<Identifier, QString>& IntercomModelMaker::supportedClasses() {
    if (!desc.isEmpty())
        return desc;

    desc["Area"] =
    "An @F Area object keeps track of the area (cm @Sup 2 per plant) which carries out photosynthesis according to its parameters and sub-models. "
    "It must hold three sub-models as children, named @F {density}, @F amax and @F {lightUseEfficiency}. The latter two specify the shape "
    "of the light response curve and the first the shape of the canopy.";

    desc["AreaDensityEven"] =
    "This model can serve as a @F density sub-model for an @F Area object. The density is the same at all heights from zero to canopy height.";

    desc["AreaDensitySymmetric"] =
    "This model can serve as a @F density sub-model for an @F Area object. The density is parabolic from zero to canopy height.";

    desc["AreaDensityTapering"] =
    "This model can serve as a @F density sub-model for an @F Area object. The density increases linearly from zero at canopy height to its maximum at zero height.";

    desc["AreaDensityTopheavy"] =
    "This model can serve as a @F density sub-model for an @F Area object. The density increases non-linearly from zero to canopy height, "
    "resulting in a topheavy shape typical of flowers";

    desc["AssimilationMaxGivenTemp"] =
    "This model can serve as the @F amax sub-model of an @F Area object. Based on the @F Tday variable of @F weather model, it calculates the maximum assimilation rate as "
    "@Math{ maxAmax (1 - radix sup Tday)}";

    desc["Community"] =
    "A @F Community model holds one or more @F Plant child models. It resolves the competition for light in daily time steps, calculating total light absorption "
    "and CO @Sub 2 assimilation of the whole canopy (i.e. of all its @F Plant objects) and dividing the resulting total among its @F Plant objects."
    "@PP If more than one @F Community object is present they will work in parallel without any interference (e.g. competition for light). "
    "Thus different scenarios can be compared in one simulation run.";

    desc["EarlyGrowth"] =
    "The @F EarlyGrowth model is a simple expontial curve based on photothermal time. "
    "A @F photothermal time model must be available as a sibling.";

    desc["Height"] =
    "A @F Plant model must have one child model named @F {height} with a @F height pull variable returning the height. "
    "The @F Height class provides the standard height model of INTERCOM which is a logistic growth curve working on photothermal time."
    "@PP @Math{height(t) = h0 + {hmax - h0} over {1 + exp(-slope(t - tm))}} @LP "
    "The @F height model looks for the nearest model named @F time to provide the daily time increment. "
    "A @F UniSim::PhotoThermalTime model can be used for this purpose (see UniSim::PhotoThermalTime @CrossLink {page @PageOf UniSim::PhotoThermalTime}).";

    desc["InstantMortality"] =
    "List of instant mortalities [0;1] applied on a certain day and month to the listed plant organs, for example, "
    "@F {((leaves 0)(stem 0.05)(flowers 0))}. Plant organs not mentioned will not be afflicted. Those mentioned must exist inside @F {Plant}.";

    desc["LightUseEfficiencyGivenTemp"] =
    "This model can serve as the @F lightUseEfficiency child model of an @F Area object. It calculates light use (or light conversion) effiency as a linear relation on "
    "@F Tday of the @F weather model";

    desc["Mass"] =
    "This model can serve as a @F mass child model of an @F Organ model. "
    "It holds iself a @F mass child model to hold the actual mass (g/plant) which is usually "
    "of the UniSim::Stage class to simulate an ageing mass";

    desc["Organ"] =
    "An @F Organ model is a child of a @F Plant model. It contains one child model called @F mass and optionally an additional model of the @F Area class";

    desc["PartitioningFixed"] =
    "This model supplies a partitioning coefficient with a fixed value";

    desc["PartitioningFixedByStage"] =
    "Pending";

    desc["Phenology"] =
    "The model of plant phenology must be a child of a @F Plant model. It may consist of any number of @F UniSim::Stage models";

    desc["Plant"] =
    "A @F Plant model is a child of the @F Community model. It holds one or more @F Organ child models and "
    "additional child models called @F {time}, @F {height} and @F {earlyGrowth}. ";

    desc["SpecificLeafArea"] =
    "This can serve as the @F specificLeafArea model of an @F Area model. It models specific leaf area (m @Sup {2}/g) as a linear relation on photothermal time, "
    "supplied by the nearest model called @F {time}, for example a @F UniSim::PhotoThermalTime model.";

    desc["Weather"] =
    "The model named @F weather of this class must be present, as several other models rely on daily "
    "weather recordings. The @F weather model holds a @F UniSim::Records inside which manages the daily "
    "readings from a weather file. This weather file must hold at least four columns labelled: Date, Tmin, "
    "Tmax and irradiationMJ. In addition to these four, @F Weather itself adds additional pull variables. "
    "@PP Values of irradiation rates pertain to the current time of the day, as set by the @F clock "
    "object";

    return desc;
}

Model* IntercomModelMaker::create(Identifier modelType, Identifier objectName, QObject *parent)
{
	setSimulationObjectFromDescendent(parent);
	Model *model = 0;
    if (modelType.equals("Area"))
        model = new Area(objectName, parent);
    else if (modelType.equals("AreaDensityEven"))
        model = new AreaDensityEven(objectName, parent);
    else if (modelType.equals("AreaDensitySymmetric"))
        model = new AreaDensitySymmetric(objectName, parent);
    else if (modelType.equals("AreaDensityTapering"))
        model = new AreaDensityTapering(objectName, parent);
    else if (modelType.equals("AreaDensityTopheavy"))
        model = new AreaDensityTopheavy(objectName, parent);
    else if (modelType.equals("AssimilationMaxGivenTemp"))
        model = new AssimilationMaxGivenTemp(objectName, parent);
    else if (modelType.equals("Community"))
        model = new Community(objectName, parent);
    else if (modelType.equals("EarlyGrowth"))
        model = new EarlyGrowth(objectName, parent);
    else if (modelType.equals("Height"))
        model = new Height(objectName, parent);
    else if (modelType.equals("InstantMortality"))
        model = new InstantMortality(objectName, parent);
    else if (modelType.equals("LightUseEfficiencyGivenTemp"))
        model = new LightUseEfficiencyGivenTemp(objectName, parent);
    else if (modelType.equals("Mass"))
        model = new Mass(objectName, parent);
    else if (modelType.equals("Organ"))
        model = new Organ(objectName, parent);
    else if (modelType.equals("PartitioningFixed"))
        model = new PartitioningFixed(objectName, parent);
    else if (modelType.equals("PartitioningFixedByStage"))
        model = new PartitioningFixedByStage(objectName, parent);
    else if (modelType.equals("Phenology"))
        model = new Phenology(objectName, parent);
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
