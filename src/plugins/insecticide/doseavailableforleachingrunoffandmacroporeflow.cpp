/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "doseavailableforleachingrunoffandmacroporeflow.h"
#include <cmath>
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (doseavailableforleachingrunoffandmacroporeflow)
	
doseavailableforleachingrunoffandmacroporeflow::doseavailableforleachingrunoffandmacroporeflow(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, sdDoseaflrm, 0.);       //dose remained after soil degradation (kg a.i/ha)
    Input (double, svDoseaflrm, 0.);       //dose remained after soil volatilization (kg a.i/ha)

    Output (double, Doseaflrm); //dose available for leaching, runoff and macropore flow (kg a.i/ha)

}

void doseavailableforleachingrunoffandmacroporeflow::reset() {

    Doseaflrm = 0.;
}

void doseavailableforleachingrunoffandmacroporeflow::update() {

    Doseaflrm = sdDoseaflrm + svDoseaflrm;

}

} //namespace

