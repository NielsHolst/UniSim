/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/variable.h>
#include "reproduction_demand_size_dependent.h"
#include "life_stage.h"

using namespace UniSim;


namespace SupplyDemand {

ReproductionDemandSizeDependent::ReproductionDemandSizeDependent(UniSim::Identifier name, QObject *parent)
    : ReproductionDemand(name, parent)
{
    new Parameter<double>("massThreshold", &massThreshold, 0.004, this,
                          "Lower threshold on individual biomass (mg) for reproduction");
    new Parameter<double>("eggProduction", &eggProduction, 10000., this,
                          "Number of eggs laid per female mass (mg) above @F massThreshold through her life time");
}

void ReproductionDemandSizeDependent::initialize() {
    ReproductionDemand::initialize();
    Model *stage = seekNearestAscendant<LifeStage*>("*");
    Model *lifeTable = stage->seekOneDescendant<Model*>("lifetable");
    individualMass = lifeTable->pullValuePtr<double>("individualMass");
}

void ReproductionDemandSizeDependent::update() {
    double m = *individualMass - massThreshold;
    if (m < 0) m = 0.;
    unlaidEggs->pushValue<double>("inflow", m*sexRatio*eggProduction);
    unlaidEggs->deepUpdate();
    value = *laidEggs*eggWeight;
}

} //namespace

