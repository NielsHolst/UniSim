/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "frogmalformation.h"
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (frogmalformation)
	
frogmalformation::frogmalformation(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, concentration, 1.);     //concentration of exposure in mg a.i/L

    Output (double, malformation);    //per cent malformation
}

void frogmalformation::reset() {

    malformation = 0.;

}

void frogmalformation::update() {

    malformation = 72.196 * concentration + 21.821;

}

} //namespace

