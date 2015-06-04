/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "retardationfactor.h"
#include <cmath>
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (retardationfactor)
	
retardationfactor::retardationfactor(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, pb, 1.365);    //soil bulk density 1.365 kg/L
    Input (double, fw, 0.25);   //soil water content fraction
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

    Output (double, Rf1);
    Output (double, Rf2);
    Output (double, Rf3);
    Output (double, Rf4);
    Output (double, Rf5);
    Output (double, Rf6);
    Output (double, Rf7);
    Output (double, Rf8);
    Output (double, Rf9);
    Output (double, Rf10);

}

void retardationfactor::reset() {

    Rf1 = 0.;
    Rf2 = 0.;
    Rf3 = 0.;
    Rf4 = 0.;
    Rf5 = 0.;
    Rf6 = 0.;
    Rf7 = 0.;
    Rf8 = 0.;
    Rf9 = 0.;
    Rf10 = 0.;

}

void retardationfactor::update() {

    Rf1 = 1. + ((pb/fw)*pHcorrectedSorptionLayer1);
    //pHcorrectedSorptionLayer1 = (Koc * focLayer1)/(1. + std::pow(10,(pHLayer1 - pKa)));

    Rf2 = 1. + ((pb/fw)*pHcorrectedSorptionLayer2);
    Rf3 = 1. + ((pb/fw)*pHcorrectedSorptionLayer3);
    Rf4 = 1. + ((pb/fw)*pHcorrectedSorptionLayer4);
    Rf5 = 1. + ((pb/fw)*pHcorrectedSorptionLayer5);
    Rf6 = 1. + ((pb/fw)*pHcorrectedSorptionLayer6);
    Rf7 = 1. + ((pb/fw)*pHcorrectedSorptionLayer7);
    Rf8 = 1. + ((pb/fw)*pHcorrectedSorptionLayer8);
    Rf9 = 1. + ((pb/fw)*pHcorrectedSorptionLayer9);
    Rf10 = 1. + ((pb/fw)*pHcorrectedSorptionLayer10);

}

} //namespace

