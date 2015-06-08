/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "publish.h"
#include "sheep_fecundity.h"

using namespace UniSim;

namespace rvf {

PUBLISH(SheepFecundity)

SheepFecundity::SheepFecundity(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, adultDensity, 10.);      // Density of adults sheep per km2
    Input(double, sexRatio, 0.1);          // Proportion of females
    Input(double, rate, 0.001);            // Number of foestus per year, gestation period per 356 days per 365 days of a year
    Input(double, carryingCapacity, 200.); // Maximum sheep density per km2
    Output(double, value);
    /// add lamb density and then make carrying capcity equal to lamb density plus adult density!!!
}

void SheepFecundity::reset() {
    value = 0;
}

void SheepFecundity::update() {
    value = rate/365*adultDensity*sexRatio*(carryingCapacity - adultDensity)/carryingCapacity;
    if (value < 0.)
        value = 0.;
}


} //namespace

