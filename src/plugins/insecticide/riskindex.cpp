/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "riskindex.h"
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (riskindex)
	
riskindex::riskindex(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, NOAEL, 0.);             //estimated lowest daily exposure level for animals (g per kg per day)
    Input (double, concentration, 0.);     //concentration of parent and metabolite per day"

    Output (double, HQ);                   //Hazard quotient index (unit)

}

void riskindex::reset() {

    HQ = 0.;

}

void riskindex::update() {

    HQ = concentration/NOAEL;

}

} //namespace

