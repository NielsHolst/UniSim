/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#include "mosquito_infection.h"

using namespace UniSim;

namespace rvf {

MosquitoInfection::MosquitoInfection(Identifier name, QObject *parent)
     : Model(name, parent)
{
    new Parameter<double>("bloodmeal", &bloodmeal, 0.2, this, "Blood meals per day");
    new Parameter<double>("transovarial", &transovarial, 0.5, this, "Transovarial transmission in mosquitoes");
    new Parameter<double>("infectionRate", &infectionRate, 0., this, "Effective contact rate per day");
    new Parameter<double>("duration", &duration, 0., this, "Infectious period in days");
    new Parameter<double>("density", &density, 0., this, "Density of adults");
    new Variable<double>("value", &value,this, "Total demand of the population");
}
void MosquitoInfection::reset() {
    value = 0.;
}
void MosquitoInfection::update() {
    value = bloodmeal * infectionRate;
}
} //namespace
