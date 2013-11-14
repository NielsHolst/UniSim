/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#include "sheep_infection.h"

using namespace UniSim;

namespace rvf {

SheepInfection::SheepInfection(Identifier name, QObject *parent)
     : Model(name, parent)
{
    new Parameter<double>("exposureRate", &exposureRate, 0., this, "Effective exposure rate per day");// Susceptibility rate
    new Parameter<double>("infectionRate", &infectionRate, 0., this, "Effective contact rate per day");
    new Parameter<double>("mortalityRate", &mortalityRate, 0., this, "sheep death rate due to disease per day");
    new Parameter<double>("density", &density, 0., this, "sheep population density");
    new Parameter<double>("duration", &duration, 0., this, "Infectious period in days");
    new Variable<double>("mortality", &mortality,this, "Number died");
}
void SheepInfection::reset() {
    mortality = 0.;
}
void SheepInfection::update() {
    mortality = infectionRate*density;
}
} //namespace
