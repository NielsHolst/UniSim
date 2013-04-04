/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "infection.h"

using namespace UniSim;

namespace strawberry {
	
Infection::Infection(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("inoculumDensity", &inoculumDensity, 0., this, "desc");
    new Parameter<double>("infectionRate", &infectionRate, 0., this, "desc");
    new Parameter<QString>("sourceModel", &sourceModelName, QString(), this, "desc");
    new Parameter<QString>("targetModel", &targetModelName, QString(), this, "desc");
    new Variable<double>("leavesInfected", &leavesInfected, this, "Number of new leaves infected (latent) per day");
}

void Infection::initialize() {
    sourceModel = seekOne<Model*>(sourceModelName);
    targetModel = seekOne<Model*>(targetModelName);
}

void Infection::reset() {
    leavesInfected = 0.;
}

void Infection::update() {
    // Calculate infection rate
    double sourceDensity = sourceModel->pullValue<double>("value");
    leavesInfected = infectionRate*sourceDensity*inoculumDensity;
    if (leavesInfected > sourceDensity)
        leavesInfected = sourceDensity;

    // Effectuate infection
    double proportionInfected = (sourceDensity > 0.) ? leavesInfected/sourceDensity : 0.;
    sourceModel->pushValue<double>("instantLossRate", proportionInfected);
    targetModel->pushValue<double>("inflow", leavesInfected);
}


} //namespace

