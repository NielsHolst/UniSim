/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "predation.h"

namespace UniSim {


Predation::Predation(Identifier name, QObject *parent)
    : FunctionalResponseMultiway(name, parent)
{
}

void Predation::createVariables() {
    supplies.resize(numPredators);
    losses.resize(numPrey);
    for (int i = 0; i < numPredators; ++i) {
        QString predatorName = predatorNames[i];
        for (int j = 0; j < numPrey; ++j) {
            QString preyName = preyNames[j];
            QString name = QString("%1-%2").arg(predatorName).arg(preyName);
            addVariable<double>(name, &attacks(i,j), "Number killed");
        }
        QString nameSupply = QString("%1-Supply").arg(predatorName);
        addVariable<double>(nameSupply, &supplies[i], "Total number killed");
    }
    for (int j = 0; j < numPrey; ++j) {
        QString preyName = preyNames[j];
        QString nameLoss = QString("%1-Loss").arg(preyName);
        addVariable<double>(nameLoss, &losses[j], "Total number lost");
    }
}

void Predation::updateFromNumAttacks() {
    supplies.fill(0.);
    losses.fill(0.);
    for (int i = 0; i < numPredators; ++i) {
        for (int j = 0; j < numPrey; ++j) {
            supplies[i] += attacks(i,j);
            losses[j] += attacks(i,j);
        }
    }
}

} //namespace
