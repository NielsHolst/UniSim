/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "bioavailabilitycorrection.h"
#include <cmath>
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (bioavailabilitycorrection)
	
bioavailabilitycorrection::bioavailabilitycorrection(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, ps, 2.65);    //density of solid materials, default 2.65 kg/L
    Input (double, Sp, 0.25);    //soil porosity
    Input (double, pHcorrectedSorptionLayer1, 0.);
    Input (double, pHcorrectedSorptionLayer2, 0.);
    Input (double, pHcorrectedSorptionLayer3, 0.);
    Input (double, pHcorrectedSorptionLayer4, 0.);
    Input (double, pHcorrectedSorptionLayer5, 0.);
    Input (double, pHcorrectedSorptionLayer6, 0.);
    Input (double, pHcorrectedSorptionLayer7, 0.);
    Input (double, pHcorrectedSorptionLayer8, 0.);
    Input (double, pHcorrectedSorptionLayer9, 0.);
    Input (double, pHcorrectedSorptionLayer10, 0.);

    Output (double, fba1);
    Output (double, fba2);
    Output (double, fba3);
    Output (double, fba4);
    Output (double, fba5);
    Output (double, fba6);
    Output (double, fba7);
    Output (double, fba8);
    Output (double, fba9);
    Output (double, fba10);

}

void bioavailabilitycorrection::reset() {

    fba1 = 0.;
    fba2 = 0.;
    fba3 = 0.;
    fba4 = 0.;
    fba5 = 0.;
    fba6 = 0.;
    fba7 = 0.;
    fba8 = 0.;
    fba9 = 0.;
    fba10 = 0.;

}

void bioavailabilitycorrection::update() {

    fba1 = 1./(1.+(pHcorrectedSorptionLayer1*ps*((1. - Sp)/Sp)));
    fba2 = 1./(1.+(pHcorrectedSorptionLayer2*ps*((1. - Sp)/Sp)));
    fba3 = 1./(1.+(pHcorrectedSorptionLayer3*ps*((1. - Sp)/Sp)));
    fba4 = 1./(1.+(pHcorrectedSorptionLayer4*ps*((1. - Sp)/Sp)));
    fba5 = 1./(1.+(pHcorrectedSorptionLayer5*ps*((1. - Sp)/Sp)));
    fba6 = 1./(1.+(pHcorrectedSorptionLayer6*ps*((1. - Sp)/Sp)));
    fba7 = 1./(1.+(pHcorrectedSorptionLayer7*ps*((1. - Sp)/Sp)));
    fba8 = 1./(1.+(pHcorrectedSorptionLayer8*ps*((1. - Sp)/Sp)));
    fba9 = 1./(1.+(pHcorrectedSorptionLayer9*ps*((1. - Sp)/Sp)));
    fba10 = 1./(1.+(pHcorrectedSorptionLayer10*ps*((1. - Sp)/Sp)));

}

} //namespace

