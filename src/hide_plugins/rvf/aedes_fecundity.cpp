/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "aedes_fecundity.h"

using namespace UniSim;

namespace rvf {
	
AedesFecundity::AedesFecundity(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("fecundityPerBloodmeal", &fecundityPerBloodmeal, 120., this, "Number of eggs laid per bloodmeal per female");
    new Parameter<double>("sexRatio", &sexRatio, 0.5, this, "Proportion of females");
    new Parameter<double>("bloodmeal", &bloodmeal, 0.2, this, "Supply of blood meals per day");
    new Parameter<double>("density", &density, 0., this, "Density of adults");
    new Variable<double>("value", &value, this, "desc");
}

void AedesFecundity::reset() {
    value = 0;
}

void AedesFecundity::update() {
    value = fecundityPerBloodmeal * bloodmeal * density * sexRatio;
}


} //namespace

