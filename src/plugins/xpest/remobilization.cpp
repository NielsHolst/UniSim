/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "crop_component.h"
#include "publish.h"
#include "remobilization.h"

using namespace UniSim;

namespace xpest {
	
PUBLISH(Remobilization)

Remobilization::Remobilization(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, currentGs, "stage[growthStage]");
    Input(QString, from, QString());    // Name of source crop component
    Input(QString, to, QString());      // Name of receiving crop component
    Input(double, proportion, 0.);      // Proportion to transfer, calculated atGs
    Input(double, atGs, 1.);            // Beginning at growth stage
    Input(double, untilGs, 2.);         // Ending at growth stage

    Output(double, rate);               // Mobilisation rate (g/m2/d)
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

