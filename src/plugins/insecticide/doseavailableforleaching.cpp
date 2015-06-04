/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "doseavailableforleaching.h"
#include <cmath>
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (doseavailableforleaching)
	
doseavailableforleaching::doseavailableforleaching(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, Doseaflm, 0.);       //dose remained in the soil after soil degradation and soil volatilization (kg a.i/ha)
    Input (double, Dmacropore, 0.);         //dose of pesticide macropore (kg a.i/ha)

    Output (double, Doseafl); //dose available for leaching (kg a.i/ha)

}

void doseavailableforleaching::reset() {

    Doseafl = 0.;
}

void doseavailableforleaching::update() {

    Doseafl = Doseaflm - Dmacropore;

}

} //namespace

