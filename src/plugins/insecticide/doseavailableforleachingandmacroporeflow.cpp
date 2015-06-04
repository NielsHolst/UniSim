/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "doseavailableforleachingandmacroporeflow.h"
#include <cmath>
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (doseavailableforleachingandmacroporeflow)
	
doseavailableforleachingandmacroporeflow::doseavailableforleachingandmacroporeflow(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, Doseaflrm, 0.);       //dose remained in the soil after soil degradation and soil volatilization (kg a.i/ha)
    Input (double, Doseldlobr, 0.);       //dose remained on leaft/washed-off during the first rainfall even (kg a.i/ha)
    Input (double, Dosesr, 0.);         //dose of pesticide runoff (kg a.i/ha)

    Output (double, Doseaflm); //dose available for leaching, runoff and macropore flow (kg a.i/ha)

}

void doseavailableforleachingandmacroporeflow::reset() {

    Doseaflm = 0.;
}

void doseavailableforleachingandmacroporeflow::update() {

    Doseaflm = (Doseaflrm + Doseldlobr) - Dosesr;

}

} //namespace

