/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "apparentdiffusionanddispersioncoefficient.h"
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (apparentdiffusionanddispersioncoefficient)
	
apparentdiffusionanddispersioncoefficient::apparentdiffusionanddispersioncoefficient(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, apparentdiffusioncoefficient, 0.25);     //effective diffusivity (m2/d)
    Input (double, porewatervelocity, 0.1); //  m/d

    Output (double, apparentDiffusionAndDispersionCoeff); // m2/d

}

void apparentdiffusionanddispersioncoefficient::reset() {

    apparentDiffusionAndDispersionCoeff = 0.;

}

void apparentdiffusionanddispersioncoefficient::update() {

    apparentDiffusionAndDispersionCoeff = apparentdiffusioncoefficient + porewatervelocity;

}

} //namespace

