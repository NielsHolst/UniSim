/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "frogpopulationmalformed.h"
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (frogpopulationmalformed)
	
frogpopulationmalformed::frogpopulationmalformed(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, currentpopulation, 1.); //current total population before malformation
    Input (double, malformation, 1.);       //per cent malformation

    Output (double, populationmalformed);    //total population malformed
}

void frogpopulationmalformed::reset() {

    populationmalformed = 0.;

}

void frogpopulationmalformed::update() {

    populationmalformed = currentpopulation * malformation/100.;

}

} //namespace

