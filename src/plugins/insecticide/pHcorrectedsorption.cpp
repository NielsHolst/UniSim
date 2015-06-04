/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "pHcorrectedsorption.h"
#include <cmath>
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (pHcorrectedsorption)
	
pHcorrectedsorption::pHcorrectedsorption(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, Koc, 9.);    //can varry from 9 - 60,000 L/kg
    Input (double, pH, 7.);
    Input (double, pKa, 0.8);   //The acid dissociation constants for glyphosate are pKa1 0.8 (1st phosphonic), pKa2 2.3 (carboxylate), pKa3 6.0 (2nd phosphonic), and pKa4 11.0 (amine).
    Input (double, focLayer1, 0.);
    Input (double, focLayer2, 0.);
    Input (double, focLayer3, 0.);
    Input (double, focLayer4, 0.);
    Input (double, focLayer5, 0.);
    Input (double, focLayer6, 0.);
    Input (double, focLayer7, 0.);
    Input (double, focLayer8, 0.);
    Input (double, focLayer9, 0.);
    Input (double, focLayer10, 0.);
    Input (double, pHLayer1, 0.);
    Input (double, pHLayer2, 0.);
    Input (double, pHLayer3, 0.);
    Input (double, pHLayer4, 0.);
    Input (double, pHLayer5, 0.);
    Input (double, pHLayer6, 0.);
    Input (double, pHLayer7, 0.);
    Input (double, pHLayer8, 0.);
    Input (double, pHLayer9, 0.);
    Input (double, pHLayer10, 0.);

    Output (double, pHcorrectedSorptionLayer1);
    Output (double, pHcorrectedSorptionLayer2);
    Output (double, pHcorrectedSorptionLayer3);
    Output (double, pHcorrectedSorptionLayer4);
    Output (double, pHcorrectedSorptionLayer5);
    Output (double, pHcorrectedSorptionLayer6);
    Output (double, pHcorrectedSorptionLayer7);
    Output (double, pHcorrectedSorptionLayer8);
    Output (double, pHcorrectedSorptionLayer9);
    Output (double, pHcorrectedSorptionLayer10);

}

void pHcorrectedsorption::reset() {

    pHcorrectedSorptionLayer1 = 0.;
    pHcorrectedSorptionLayer2 = 0.;
    pHcorrectedSorptionLayer3 = 0.;
    pHcorrectedSorptionLayer4 = 0.;
    pHcorrectedSorptionLayer5 = 0.;
    pHcorrectedSorptionLayer6 = 0.;
    pHcorrectedSorptionLayer7 = 0.;
    pHcorrectedSorptionLayer8 = 0.;
    pHcorrectedSorptionLayer9 = 0.;
    pHcorrectedSorptionLayer10 = 0.;

}

void pHcorrectedsorption::update() {

    pHcorrectedSorptionLayer1 = (Koc * focLayer1)/(1. + std::pow(10,(pHLayer1 - pKa)));
    pHcorrectedSorptionLayer2 = (Koc * focLayer2)/(1. + std::pow(10,(pHLayer2 - pKa)));
    pHcorrectedSorptionLayer3 = (Koc * focLayer3)/(1. + std::pow(10,(pHLayer3 - pKa)));
    pHcorrectedSorptionLayer4 = (Koc * focLayer4)/(1. + std::pow(10,(pHLayer4 - pKa)));
    pHcorrectedSorptionLayer5 = (Koc * focLayer5)/(1. + std::pow(10,(pHLayer5 - pKa)));
    pHcorrectedSorptionLayer6 = (Koc * focLayer6)/(1. + std::pow(10,(pHLayer6 - pKa)));
    pHcorrectedSorptionLayer7 = (Koc * focLayer7)/(1. + std::pow(10,(pHLayer7 - pKa)));
    pHcorrectedSorptionLayer8 = (Koc * focLayer8)/(1. + std::pow(10,(pHLayer8 - pKa)));
    pHcorrectedSorptionLayer9 = (Koc * focLayer9)/(1. + std::pow(10,(pHLayer9 - pKa)));
    pHcorrectedSorptionLayer10 = (Koc * focLayer10)/(1. + std::pow(10,(pHLayer10 - pKa)));

}

} //namespace

