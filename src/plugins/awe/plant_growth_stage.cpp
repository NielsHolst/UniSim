/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QString>
#include <usbase/variable.h>
#include <usbase/utilities.h>
#include "../unisim/stage.h"
#include "plant_growth_stage.h"
#include "publish.h"

using namespace UniSim;

namespace awe {

PUBLISH(PlantGrowthStage)

PlantGrowthStage::PlantGrowthStage(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    setRecursionPolicy(Component::Reset,  Component::ChildrenLast);
    setRecursionPolicy(Component::Update, Component::ChildrenNot);

    Input(double, inflowAsDensity, 0.); /*
                     "Number of plants to enter this growth stage in the next update
                     "(i.e. time step) (plants per m @Sup 2 per day */
    Input(double, inflowAsDensityEqs, 0.); /*
                     "Number of density equivalents to enter this growth stage in the next update
                     "(i.e. time step) (density equivalents per m @Sup 2 per day) */
    Input(double, instantMortality, 0.); /*
                     "Instant mortality (0-100%) to apply to this growth stage in the next update */
    Output(double, outflowAsDensity); /*
                     "Number of plants that emerged from this growth stage during the last update "
                     "(i.e. time step) (plants per m @Sup 2 per day) */
    Output(double, outflowAsDensityEqs); /*
                     "Number of density equivalents that emerged from this growth stage during the last update "
                     "(i.e. time step) (density equivalents per m @Sup 2 per day) */
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
    density->pushValue("inflow", inflowAsDensity);
    densityEqs->pushValue("inflow", inflowAsDensityEqs);

    density->pushValue("instantMortality", instantMortality);
    densityEqs->pushValue("instantMortality", instantMortality);
    instantMortality = 0.;

    density->deepUpdate();
    densityEqs->deepUpdate();

    outflowAsDensity = density->pullValue<double>("outflow");
    outflowAsDensityEqs = densityEqs->pullValue<double>("outflow");
}

}
