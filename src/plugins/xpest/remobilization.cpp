/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "crop_component.h"
#include "remobilization.h"

using namespace UniSim;

namespace xpest {
	
Remobilization::Remobilization(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<QString>(Name(from), QString(), "Name of source crop component");
    addParameter<QString>(Name(to), QString(), "Name of receiving crop component");
    addParameter<double>(Name(proportion), 0., "Proportion to transfer, calculated atGs");
    addParameter<double>(Name(atGs), 1., "Beginning at growth stage");
    addParameter<double>(Name(untilGs), 2., "Ending at growth stage");
    addParameterRef<double>(Name(currentGs), "stage[growthStage]");

    addVariable<double>(Name(rate), "Mobilisation rate (g/m2/d)");
}

void Remobilization::amend() {
    fromComponent = seekOne<CropComponent*>(from);
    toComponent = seekOne<CropComponent*>(to);
}

void Remobilization::reset() {
    rate = 0.;
    prevGs = currentGs;
    inEffect = false;
}

void Remobilization::update() {
    if (!inEffect && currentGs >= atGs)
        setLine();
    rate = (currentGs > atGs && currentGs <= untilGs) ?
                -slope*(currentGs - prevGs) : 0.;
    inEffect = inEffect && currentGs < untilGs;
    prevGs = currentGs;
    effectuate();
}

void Remobilization::setLine() {
    double amount = fromComponent->pullValue<double>("mass");
    slope = -proportion*amount/(untilGs - atGs);
    inEffect = true;
}

void Remobilization::effectuate() {
    fromComponent->pushValue<double>("remobilizationOut", rate);
    toComponent->pushValue<double>("remobilizationIn", rate);
}

} //namespace

