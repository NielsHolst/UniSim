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
#include "intercom_factory.h"
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

void IntercomFactory::defineProducts() {
    addProduct<Area>("Area", this,
    "An @F Area object keeps track of the area (cm @Sup 2 per plant) which carries out photosynthesis according to its parameters and sub-models. "
    "It must hold three sub-models as children, named @F {density}, @F amax and @F {lightUseEfficiency}. The latter two specify the shape "
    "of the light response curve and the first the shape of the canopy.");

    addProduct<AreaDensityEven>("AreaDensityEven", this,
    "This model can serve as a @F density sub-model for an @F Area object. "
    "The density is the same at all heights from zero to canopy height.");

    addProduct<AreaDensitySymmetric>("AreaDensitySymmetric", this,
    "This model can serve as a @F density sub-model for an @F Area object. "
    "The density is parabolic from zero to canopy height.");

    addProduct<AreaDensityTapering>("AreaDensityTapering", this,
    "This model can serve as a @F density sub-model for an @F Area object. "
    "The density increases linearly from zero at canopy height to its maximum at zero height.");

    addProduct<AreaDensityTopheavy>("AreaDensityTopheavy", this,
    "This model can serve as a @F density sub-model for an @F Area object. "
    "The density increases non-linearly from zero to canopy height, "
    "resulting in a topheavy shape typical of flowers");

    addProduct<AssimilationMaxGivenTemp>("AssimilationMaxGivenTemp", this,
    "This model can serve as the @F amax sub-model of an @F Area object. Based on the @F Tday variable of @F weather model, "
    "it calculates the maximum assimilation rate as @Math{ maxAmax (1 - radix sup Tday)}");

    addProduct<Community>("Community", this,
    "A @F Community model holds one or more @F Plant child models. It resolves the competition for light in daily time steps, calculating total light absorption "
    "and CO @Sub 2 assimilation of the whole canopy (i.e. of all its @F Plant objects) and dividing the resulting total among its @F Plant objects."
    "@PP If more than one @F Community object is present they will work in parallel without any interference (e.g. competition for light). "
    "Thus different scenarios can be compared in one simulation run.");

    addProduct<EarlyGrowth>("EarlyGrowth", this,
    "The @F EarlyGrowth model is a simple expontial curve based on photothermal time. "
    "A @F photothermal time model must be available as a sibling.");

    addProduct<Height>("Height", this,
    "A @F Plant model must have one child model named @F {height} with a @F height pull variable returning the height. "
    "The @F Height class provides the standard height model of INTERCOM which is a logistic growth curve working on photothermal time."
    "@PP @Math{height(t) = h0 + {hmax - h0} over {1 + exp(-slope(t - tm))}} @LP "
    "The @F height model looks for the nearest model named @F time to provide the daily time increment. "
    "A @F UniSim::PhotoThermalTime model can be used for this purpose (see UniSim::PhotoThermalTime @CrossLink {page @PageOf UniSim::PhotoThermalTime}).");

    addProduct<InstantMortality>("InstantMortality", this,
    "List of instant mortalities [0;1] applied on a certain day and month to the listed plant organs, for example, "
    "@F {((leaves 0)(stem 0.05)(flowers 0))}. Plant organs not mentioned will not be afflicted. Those mentioned must exist inside @F {Plant}.");

    addProduct<LightUseEfficiencyGivenTemp>("LightUseEfficiencyGivenTemp", this,
    "This model can serve as the @F lightUseEfficiency child model of an @F Area object. "
    "It calculates light use (or light conversion) effiency as a linear relation on "
    "@F Tday of the @F weather model");

    addProduct<Mass>("Mass", this,
    "This model can serve as a @F mass child model of an @F Organ model. "
    "It holds iself a @F mass child model to hold the actual mass (g/plant) which is usually "
    "of the UniSim::Stage class to simulate an ageing mass");

    addProduct<Organ>("Organ", this,
    "An @F Organ model is a child of a @F Plant model. "
    "It contains one child model called @F mass and optionally an additional model of the @F Area class");

    addProduct<PartitioningFixed>("PartitioningFixed", this,
    "This model supplies a partitioning coefficient with a fixed value");

    addProduct<PartitioningFixedByStage>("PartitioningFixedByStage", this,
    "Pending");

    addProduct<Phenology>("Phenology", this,
    "The model of plant phenology must be a child of a @F Plant model. It may consist of any number of @F UniSim::Stage models");

    addProduct<Plant>("Plant", this,
    "A @F Plant model is a child of the @F Community model. It holds one or more @F Organ child models and "
    "additional child models called @F {time}, @F {height} and @F {earlyGrowth}. ");

    addProduct<SpecificLeafArea>("SpecificLeafArea", this,
    "This can serve as the @F specificLeafArea model of an @F Area model. It models specific leaf area (m @Sup {2}/g) as a linear relation on photothermal time, "
    "supplied by the nearest model called @F {time}, for example a @F UniSim::PhotoThermalTime model.");

    addProduct<Weather>("Weather", this,
    "The model named @F weather of this class must be present, as several other models rely on daily "
    "weather recordings. The @F weather model holds a @F UniSim::Records inside which manages the daily "
    "readings from a weather file. This weather file must hold at least four columns labelled: Date, Tmin, "
    "Tmax and irradiationMJ. In addition to these four, @F Weather itself adds additional pull variables. "
    "@PP Values of irradiation rates pertain to the current time of the day, as set by the @F clock "
    "object");
}

Identifier IntercomFactory::id() const {
    return "intercom";
}

QString IntercomFactory::description() const {
    return
    "The @F intercom plugin is an implementation of the INTERCOM model for plant growth and competition, "
    "as described by @Cite{$label{Kropff \"&\" Laar (1993)}kropff_laar}.";
}

QStringList IntercomFactory::authors() const {
    return QStringList()
        << "Niels" << "Marleen" << "Jon";
}
QObject* IntercomFactory::asQObject() {
    return this;
}

Q_EXPORT_PLUGIN2(intercom_factory, IntercomFactory)

} //namespace
