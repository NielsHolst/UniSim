/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "hazardquotient.h"
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (hazardquotient)
	
hazardquotient::hazardquotient(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, lowestAvailableNOAEL, 0.);   //estimated lowest daily exposure level for animals (g per kg per day)
    Input (double, concentration, 0.);          //concentration of parent and metabolite per day"
    Input (double, RV, 1);                      //reference value <1 no risk and >1 there is risk
    Output (double, HQ);                        //Hazard quotient index (unit)
    Output (double, ReferenceValue);

}

void hazardquotient::reset() {

    concentration = 0.;
    HQ = 0.;

}

void hazardquotient::update() {
    HQ = concentration/lowestAvailableNOAEL;
    ReferenceValue = RV;

}

} //namespace

