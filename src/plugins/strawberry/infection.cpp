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
    Input(double, inoculumDensity, 0.);
    Input(double, infectionRate, 0.);
    Input2(QString, sourceModelName, sourceModel, QString());
    Input2(QString, targetModelName, targetModel, QString());
    Output(double, leavesInfected); // Number of new leaves infected (latent) per day
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

