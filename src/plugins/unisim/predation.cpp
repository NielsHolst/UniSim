/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QGenericMatrix>
#include <usbase/data_grid.h>
#include <usbase/utilities.h>
#include "predation.h"

namespace UniSim {


Predation::Predation(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<QString>("apparencyMatrix", &apparencyMatrixFileName, QString(""), this,
    "File with references to predator demands and apparency value for each predator x prey combiation");
}

void Predation::amend() {
    DataGrid am(apparencyMatrixFileName, this);
    createLinks(demands, am.rowNames());
    createLinks(resources, QStringList(am.columnNames().mid(1)));
    numPredators = demands.size();
    numPrey = resources.size();

    apparencies.resize(numPredators, numPrey);
    for (int i = 0; i < numPredators; ++i) {
        for (int j = 0; j < numPrey; ++j) {
            apparencies(i,j) = UniSim::stringToValue<double>(am.cell(i,j), this);
        }
    }
    attacks.resize(numPredators, numPrey);
}

void Predation::createLinks(Links &links, QStringList refs) {
    for (int i = 0; i < refs.size(); ++i) {
        QString name = refs[i];
        Variable<double> *variable = seekOne<Model*, Variable<double>* >(name);
        links << variable->valuePtr();
    }
}

void Predation::reset() {
    update();
}

void Predation::update() {
    updateAttacksByPredator();
}

void Predation::updateAttacksByPredator() {
    for (int i = 0; i < numPredators; ++i) {
        double D = *demands.at(i);
        double sumAttacks = 0.;
        double sumWeightedRessources = 0.;

        for (int j = 0; j < numPrey; ++j) {
            double X = *resources.at(j);
            double a = apparencies.at(i,j);
            sumAttacks += attacks(i,j) = GBFuncResp(D, a*X);
            sumWeightedRessources += a*X;
        }

        double combinedAttacks = GBFuncResp(D, sumWeightedRessources);
        for (int j = 0; j < numPrey; ++j) {
            attacks(i,j) *= (sumAttacks == 0.) ? 0. : combinedAttacks/sumAttacks;
        }
    }
}

void Predation::updateAttacksByPrey() {
    QVector<double> proportionsWanted(numPrey);
    for (int j = 0; j < numPrey; ++j) {
        double X = *resources.at(j);
        for (int i = 0; i < numPredators; ++i) {
            proportionsWanted[i] = (X == 0.) ? 0 :attacks.at(i,j)/X;
        }
    }
}


} //namespace
