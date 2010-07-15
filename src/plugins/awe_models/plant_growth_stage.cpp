/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QString>
#include <usbase/pull_variable.h>
#include <usbase/push_variable.h>
#include <usbase/utilities.h>
#include "../unisim_models/stage.h"
#include "plant_growth_stage.h"

using namespace UniSim;

namespace awe {

PlantGrowthStage::PlantGrowthStage(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    setRecursionPolicy(Component::Reset,  Component::ChildrenLast);
    setRecursionPolicy(Component::Update, Component::ChildrenNot);

    new PullVariable("outflowAsDensity", &outflowAsDensity, this,
                     "Number of plants that emerged from this growth stage during the last update "
                     "(i.e. time step) (plants per m @Sup 2 per day)");
    new PullVariable("outflowAsDensityEqs", &outflowAsDensityEqs, this,
                     "Number of density equivalents that emerged from this growth stage during the last update "
                     "(i.e. time step) (density equivalents per m @Sup 2 per day)");
    new PushVariable("inflowAsDensity", &inflowAsDensity, this,
                     "Number of plants to enter this growth stage in the next update "
                     "(i.e. time step) (plants per m @Sup 2 per day)");
    new PushVariable("inflowAsDensityEqs", &inflowAsDensityEqs, this,
                     "Number of density equivalents to enter this growth stage in the next update "
                     "(i.e. time step) (density equivalents per m @Sup 2 per day)");
    new PushVariable("instantMortality", &instantMortality, this,
                     "Instant mortality (0-100%) to apply to this growth stage in the next update ");
}

void PlantGrowthStage::initialize() {
    density = seekOneChild<Stage*>("density");
    densityEqs = seekOneChild<Stage*>("densityEqs");
}

void PlantGrowthStage::reset() {
    inflowAsDensity = 0.;
    inflowAsDensityEqs = 0.;
    instantMortality = 0.;
}

void PlantGrowthStage::update() {
    density->pushVariable("inflow", inflowAsDensity);
    densityEqs->pushVariable("inflow", inflowAsDensityEqs);

    density->pushVariable("instantMortality", instantMortality);
    densityEqs->pushVariable("instantMortality", instantMortality);
    instantMortality = 0.;

    density->deepUpdate();
    densityEqs->deepUpdate();

    outflowAsDensity = density->pullVariable("outflow");
    outflowAsDensityEqs = densityEqs->pullVariable("outflow");
}

}
