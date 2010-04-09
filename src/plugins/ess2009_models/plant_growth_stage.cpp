/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QString>
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

	setState("outflowAsDensity", &_outflowAsDensity);
	setState("outflowAsDensityEqs", &_outflowAsDensityEqs);
}

void PlantGrowthStage::initialize() {
    _density = seekOneChild<Stage*>("density");
    _densityEqs = seekOneChild<Stage*>("densityEqs");
}

void PlantGrowthStage::reset() {
	setInput("inflowAsDensity", 0.);
	setInput("inflowAsDensityEqs", 0.);
    setInput("instantMortality", 0.);
}

void PlantGrowthStage::update() {
	_density->setInput("inflow", input("inflowAsDensity"));
	_densityEqs->setInput("inflow", input("inflowAsDensityEqs"));

    _density->setInput("instantMortality", input("instantMortality"));
    _densityEqs->setInput("instantMortality", input("instantMortality"));
    setInput("instantMortality", 0.);

    _density->deepUpdate();
    _densityEqs->deepUpdate();

	_outflowAsDensity = _density->state("outflow");
	_outflowAsDensityEqs = _densityEqs->state("outflow");
}

}
