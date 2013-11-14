/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include "pollen.h"

using namespace UniSim;


namespace ecotox {

Pollen::Pollen(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("distance", &distance, 0., this,
                          "Distance to maize field (m)");
    new Parameter<double>("distanceExp", &distanceExp, -0.548, this,
                          "Pollen deposition declines with the distance from the maize field. "
                          "This is described as a power function with the exponent @F {distanceExp}");
    new Parameter<double>("distanceMin", &distanceMin, 0.3, this,
                          "Minimum distance of pollen dispersion model (m)");
    new Parameter<double>("Npeak", &peakDensity, 1750., this,
                          "Peak average density of pollen deposited on food plant (pollen per cm @Sup {2}) at field edge, i.e. "
                          "at @F {distance <= distanceMin}");

    new Variable<double>("pollenDepositionRate", &pollenDepositionRate, this,
                             "Current pollen deposition rate (per day per cm @Sup {2})");
    new Variable<double>("pollenDepositionTotal", &pollenDepositionTotal, this,
                             "Total pollen deposition (per cm @Sup {2})");
    new Variable<double>("pollenDensity", &pollenDensity, this,
                             "Current pollen density (per cm @Sup {2})");
}

void Pollen::initialize() {
    Model *flush = peekOneChild<Model*>("depositionFlush");
    if (flush) {
        depositionRate = flush->pullValuePtr<double>("value");
    }
    else {
        Model *deposition = seekOneChild<Model*>("depositionRate");
        depositionRate = deposition->pullValuePtr<double>("value");
    }
    Model *loss = seekOneChild<Model*>("lossRate");
    lossRate = loss->pullValuePtr<double>("value");
}

void Pollen::reset() {
    pollenDepositionRate =
    pollenDepositionTotal =
    pollenDensity = 0.;
}

void Pollen::update() {
    pollenDepositionRate = peakDensity*distanceEffect()*(*depositionRate);
    pollenDepositionTotal += pollenDepositionRate;
    pollenDensity *= 1. - (*lossRate);
    pollenDensity +=  pollenDepositionRate;
}

double Pollen::distanceEffect() {
    return (distance <= distanceMin) ? 1. : pow(distance, distanceExp)*pow(distanceMin, -distanceExp);
}

} //namespace

