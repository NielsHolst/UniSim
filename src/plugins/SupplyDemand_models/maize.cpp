/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include "maize.h"

using namespace UniSim;

namespace SupplyDemand {

Maize::Maize(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("pollenProduction", &pollenProduction, 25., this,
                          "Total pollen production of maize crop (g/m2)");
    new Parameter<double>("depositionRate", &depositionRate, 0.9, this,
                          "Relative deposition rate of pollen [0..1]");
    new Parameter<double>("btConcentration", &btConcentration, 1e-10, this,
                          "Concentration of Bt toxin in pollen");
}

void Maize::initialize() {
    sowing = seekOneChild<Model*>("sowing");
    vegetative = seekOneChild<Model*>("vegetative");
    pollenRelease = seekOneChild<Model*>("pollenRelease");
    btDeposited = seekOneChild<Model*>("btDeposited");
    vegetativeOutflow = vegetative->pullVariablePtr<double>("outflow");
    pollenReleased = pollenRelease->pullVariablePtr<double>("outflow");
}

void Maize::update() {
    if (sowing->pullVariable<bool>("value"))
        vegetative->pushVariable<double>("inflow", 1.);

    pollenRelease->pushVariable<double>("inflow", *vegetativeOutflow);

    double btDeposition = btConcentration*pollenProduction*depositionRate*(*pollenReleased)*1e12;
    btDeposited->pushVariable<double>("inflow", btDeposition);
}


} //namespace

