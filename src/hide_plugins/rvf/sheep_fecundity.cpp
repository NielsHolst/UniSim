/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "sheep_fecundity.h"

using namespace UniSim;

namespace rvf {
	
SheepFecundity::SheepFecundity(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("adultDensity", &adultDensity, 10., this, "Density of adults sheep per km2");
    new Parameter<double>("sexRatio", &sexRatio, 0.1, this, "Proportion of females");
    new Parameter<double>("rate", &rate, 0.001, this, "Number of foestus per year, gestation period per 356 days per 365 days of a year");
    new Parameter<double>("carryingCapacity", &carryingCapacity, 200., this, "Maximum sheep density per km2");
    new Variable<double>("value", &value, this, "desc");
    /// add lamp density and then make carrying capcity equal to lamp density plus adult density!!!
}

void SheepFecundity::reset() {
    value = 0;
}

void SheepFecundity::update() {
    value = rate/365*adultDensity*sexRatio*(carryingCapacity - adultDensity)/carryingCapacity;
    if (value < 0)
        value = 0;
}


} //namespace

