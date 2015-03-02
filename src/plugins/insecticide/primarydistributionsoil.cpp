/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "primarydistributionsoil.h"
#include <cmath>
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (primarydistributionsoil)
	
primarydistributionsoil::primarydistributionsoil(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, Doseappl, 0.);     //kg/ha
    Input (double, Doserl, 0.);
    Input (double, Dosedrift, 0.);

    Output (double, Dosers);
}

void primarydistributionsoil::reset() {

    Dosers = 0.;
}

void primarydistributionsoil::update() {

    Dosers = Doseappl - (Doserl + Dosedrift);
}

} //namespace

