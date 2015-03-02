/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "HazardQuotient.h"
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (HazardQuotient)
	
HazardQuotient::HazardQuotient(Identifier name, QObject *parent)
	: Model(name, parent)
{
//    addParameter<double>(Name(lowestAvailableNOAEL), 0., "estimated lowest daily exposure level for animals (g per kg per day)");
//    addParameter<double>(Name(concentration), 0., "concentration of parent and metabolite per day");
//    addVariable<double>(Name(HQ), "Hazard quotient index (unit)");
//    addVariable<double>(Name(ReferenceValue), "Hazard quotient reference value (unit)");
//    addParameter<double>(Name(RV), 0., "reference value <1 no risk and >1 there is risk");
    Input (double, lowestAvailableNOAEL, 0.);
    Input (double, concentration, 0.);
    Input (double, RV, 0.);
    Output (double, HQ);
    Output (double, ReferenceValue);

}

void HazardQuotient::reset() {

    concentration = 0.;
    HQ = 0.;



}

void HazardQuotient::update() {
    HQ = concentration/lowestAvailableNOAEL;
    ReferenceValue = RV;


}

} //namespace

