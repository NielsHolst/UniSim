/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "infection.h"
#include "predation.h"

namespace UniSim {

Infection::Infection(Identifier name, QObject *parent)
    : FunctionalResponseMultiway(name, parent)
{
}

void Infection::createVariables() {
    attacksPerHost.resize(numPredators, numPrey);
    numHostsAttacked.resize(numPredators, numPrey);
    propHostsAttacked.resize(numPredators, numPrey);
    for (int i = 0; i < numPredators; ++i) {
        QString predatorName = predatorNames[i];
        for (int j = 0; j < numPrey; ++j) {
            QString preyName = preyNames[j];
            QString name = QString("%1-%2-").arg(predatorName).arg(preyName);
            addVariable<double>(name+"NumAttacks", &attacks(i,j), "Number attacked");
            addVariable<double>(name+"AttacksPerHost", &attacksPerHost(i,j), "Average number of attacks per host");
            addVariable<double>(name+"NumHostsAttacked", &numHostsAttacked(i,j), "Number of hosts attacked");
            addVariable<double>(name+"PropHostsAttacked", &propHostsAttacked(i,j), "Proportion of hosts attacked");
        }
    }
}

void Infection::updateFromNumAttacks() {
    for (int i = 0; i < numPredators; ++i) {
        for (int j = 0; j < numPrey; ++j) {
            double hostDensity = *resources.at(j);
            attacksPerHost(i,j) = (hostDensity > 0.) ? attacks(i,j)/hostDensity : 0.;
            propHostsAttacked(i,j) = 1. - exp(-attacksPerHost(i,j));
            numHostsAttacked(i,j) = hostDensity*propHostsAttacked(i,j);
        }
    }
}

} //namespace
