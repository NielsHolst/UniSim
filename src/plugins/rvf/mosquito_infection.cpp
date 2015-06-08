/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "mosquito_infection.h"
#include "publish.h"

using namespace UniSim;

namespace rvf {

PUBLISH(MosquitoInfection)

MosquitoInfection::MosquitoInfection(Identifier name, QObject *parent)
     : Model(name, parent)
{
    Input(double, bloodmeal, 0.2);      // Blood meals per day
    Input(double, transovarial, 0.5);   // Transovarial transmission in mosquitoes
    Input(double, infectionRate, 0.);   // Effective contact rate per day
    Input(double, duration, 0.);        // Infectious period in days
    Input(double, density, 0.);         // Density of adults
    Output(double, value);              // Total demand of the population
}
void MosquitoInfection::reset() {
    value = 0.;
}
void MosquitoInfection::update() {
    value = bloodmeal * infectionRate;
}
} //namespace
