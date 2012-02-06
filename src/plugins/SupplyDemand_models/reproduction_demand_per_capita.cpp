/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/pull_variable.h>
#include "reproduction_demand_per_capita.h"

using namespace UniSim;


namespace SupplyDemand {

ReproductionDemandPerCapita::ReproductionDemandPerCapita(UniSim::Identifier name, QObject *parent)
    : ReproductionDemand(name, parent)
{
    new Parameter<double>("eggProduction", &eggProduction, 200., this,
                          "Number of eggs laid per female through her life time");
}

void ReproductionDemandPerCapita::update() {
    unlaidEggs->pushVariable<double>("inflow", *newAdults*sexRatio*eggProduction);
    unlaidEggs->deepUpdate();
    value = *laidEggs*eggWeight;
}

} //namespace

