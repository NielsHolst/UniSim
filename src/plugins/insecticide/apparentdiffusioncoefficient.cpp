/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "apparentdiffusioncoefficient.h"
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (apparentdiffusioncoefficient)
	
apparentdiffusioncoefficient::apparentdiffusioncoefficient(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, Bew, 0.25);     //effective diffusivity (m2/s)

    Output (double, apparentDiffusionCoeff); // m2/d

}

void apparentdiffusioncoefficient::reset() {

    apparentDiffusionCoeff = 0.;

}

void apparentdiffusioncoefficient::update() {

    apparentDiffusionCoeff = Bew * 86400.;

}

} //namespace

