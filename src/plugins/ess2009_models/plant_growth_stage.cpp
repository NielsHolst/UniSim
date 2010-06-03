/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QString>
#include <usbase/pull_variable.h>
#include <usbase/push_variable.h>
#include <usbase/utilities.h>
#include "../standard_models/stage.h"
#include "plant_growth_stage.h"

using namespace UniSim;

namespace ess2009 {

PlantGrowthStage::PlantGrowthStage(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    setRecursionPolicy(Component::Reset,  Component::ChildrenLast);
    setRecursionPolicy(Component::Update, Component::ChildrenNot);

    new PullVariable("outflowAsDensity", &_outflowAsDensity, this);
    new PullVariable("outflowAsDensityEqs", &_outflowAsDensityEqs, this);

    new PushVariable("inflowAsDensity", &inflowAsDensity, this);
    new PushVariable("inflowAsDensityEqs", &inflowAsDensityEqs, this);
    new PushVariable("instantMortality", &instantMortality, this);
}

void PlantGrowthStage::initialize() {
    _density = seekOneChild<Stage*>("density");
    _densityEqs = seekOneChild<Stage*>("densityEqs");
}

void PlantGrowthStage::reset() {
    inflowAsDensity = 0.;
    inflowAsDensityEqs = 0.;
    instantMortality = 0.;
}

void PlantGrowthStage::update() {
    _density->pushVariable("inflow", inflowAsDensity);
    _densityEqs->pushVariable("inflow", inflowAsDensityEqs);

    _density->pushVariable("instantMortality", instantMortality);
    _densityEqs->pushVariable("instantMortality", instantMortality);
    instantMortality = 0.;

    _density->deepUpdate();
    _densityEqs->deepUpdate();

    _outflowAsDensity = _density->pullVariable("outflow");
    _outflowAsDensityEqs = _densityEqs->pullVariable("outflow");
}

}
