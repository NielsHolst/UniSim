/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "pesticideinducedmortalityrate.h"
#include <cmath>
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (pesticideinducedmortalityrate)
	
pesticideinducedmortalityrate::pesticideinducedmortalityrate(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, LC50, 0.);  //constant LC50 (mg a.i/L)
    Input (double, concentration, 1.);     //predicted concentration in the soil at time t (kg a.i/ha)
    Input (double, slope, 1.);     //scale the specific toxicity of the simulated pesticide or sigmoidal function (volume/amount)
    Input(double, ar, 1.);         //application rate, L/ha

    Output (double, mortalityRate);   //per cent of eggs, tadpole, ....adult frog dies per day
    Output (double, C);   //predicted soil concentration at time t (mg a.i/L)

}

void pesticideinducedmortalityrate::reset() {

    C = 0.;
    mortalityRate = 0.;

}

void pesticideinducedmortalityrate::update() {

    C = (concentration > 0) ? (concentration * 1000000.)/ar : 0;
    mortalityRate = (1./(1. + exp(-slope*(log(C) - log(LC50)))))*100.;

}

} //namespace

