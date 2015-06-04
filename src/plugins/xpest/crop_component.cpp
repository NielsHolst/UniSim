/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "crop_component.h"
#include "publish.h"

using namespace UniSim;

namespace xpest {
	
PUBLISH(CropComponent)

CropComponent::CropComponent(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, partitioningCoefficient, "./partition[yValue]");
    InputRef(double, assimilate, "growth[gain]");
    Input(bool, isBelowGround, false);      // Is this component below ground?
    Input(double, sla, 0.);                 // Specific leaf area (m2/g)
    Input(double, initMass, 100.);          // Initial biomass per ground area (g/m2)
    Input(double, senescence, 0.);          // Proportion of daily biomass loss (per d)
    Input(double, remobilizationIn, 0.);    // Remobilised assimilated inflow (g/m2/d)
    Input(double, remobilizationOut, 0.);   // Remobilised assimilated outflow (g/m2/d)
    Input(double, assimilateLoss, 0.);      // Proportional loss of assimilates [0;1]

    Output(double, netPartition);           // Net partition [0;1]
    Output(double, mass);                   // Biomass per ground area (g/m2)
    Output(double, lai);                    // Leaf area index
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

