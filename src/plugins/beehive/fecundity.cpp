/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include "fecundity.h"

using namespace UniSim;


namespace beehive {

Fecundity::Fecundity(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("sexRatio", &sexRatio, 0.5, this, "Proportion of females");
    new Parameter<double>("rate", &rate, 100., this, "Number of eggs laid per individual per day");
    new Parameter<double>("eggMass", &eggMass, 0.01, this, "Mass of one egg (mg)");
    new Parameter<double>("sizeThreshold", &sizeThreshold, 12., this, "Minimum size of an adult to reproduce (mg)");
    new Variable<double>("number", &number, this, "Number of eggs laid by total population");
    new Variable<double>("mass", &mass, this, "Mass of eggs laid by total population");
}

void Fecundity::initialize() {
    adult = seekParent<Model*>("*");
    numberOfAdults = adult->seekOneChild<Model*>("number");
}

void Fecundity::reset() {
    number = mass = 0.;
}

void Fecundity::update() {
    number = numberOfAdults->pullValue<double>("value") * rate;
    if (adult->pullValue<double>("size") < sizeThreshold)
        number = 0;
    mass = sexRatio*number*eggMass;
}

} //namespace

