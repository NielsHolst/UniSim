/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "crop_component.h"

using namespace UniSim;

namespace xpest {
	
CropComponent::CropComponent(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(partitioningCoefficient), "./partition[yValue]");
    addParameterRef<double>(Name(assimilate), "growth[gain]");
    addParameter<bool>(Name(isBelowGround), false, "Is this component below ground?");
    addParameter<double>(Name(sla), 0., "Specific leaf area (m2/g)");
    addParameter<double>(Name(initMass), 100., "Initial biomass per ground area (g/m2)");
    addParameter<double>(Name(senescence), 0., "Proportion of daily biomass loss (per d)");
    addParameter<double>(Name(remobilizationIn), 0., "Remobilised assimilated inflow (g/m2/d)");
    addParameter<double>(Name(remobilizationOut), 0., "Remobilised assimilated outflow (g/m2/d)");
    addParameter<double>(Name(assimilateLoss), 0., "Proportional loss of assimilates [0;1]");
    addVariable<double>(Name(netPartition), "Net partition [0;1]");
    addVariable<double>(Name(mass), "Biomass per ground area (g/m2)");
    addVariable<double>(Name(lai), "Leaf area index");
}

void CropComponent::amend() {
    belowGroundPartition = seekOne<Model*>("belowGroundPartition")->pullValuePtr<double>("yValue");
}

void CropComponent::reset() {
    mass = initMass;
    updateFromMass();
}

void CropComponent::update() {
    netPartition = partitioningCoefficient *
                    (isBelowGround ? *belowGroundPartition : 1. - *belowGroundPartition);
    mass *= 1. - senescence;
    mass += netPartition*assimilate*(1. - assimilateLoss) + remobilizationIn - remobilizationOut;
    updateFromMass();
}

void CropComponent::updateFromMass() {
    lai = mass*sla;
}

} //namespace

