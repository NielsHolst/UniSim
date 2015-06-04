/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "porewatervelocity.h"
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (porewatervelocity)
	
porewatervelocity::porewatervelocity(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, fw, 0.25);     //soil water fraction, 0.25 (default)
    Input (double, filtervelocity, 0.);

    Output (double, porewatervel); // m/d

}

void porewatervelocity::reset() {

    porewatervel = 0.;

}

void porewatervelocity::update() {

    porewatervel = (filtervelocity/fw < 0) ? 0.0000000001 : (filtervelocity/fw);

}

} //namespace

