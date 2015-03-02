/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "organiccarbonwaterpartitioning.h"
#include <cmath>
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (organiccarbonwaterpartitioning)
	
organiccarbonwaterpartitioning::organiccarbonwaterpartitioning(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, Koc, 9.);    //can varry from 9 - 60,000 L/kg
    Input (double, pH, 7.);
    Input (double, pKa, 0.8);   //The acid dissociation constants for glyphosate are pKa1 0.8 (1st phosphonic), pKa2 2.3 (carboxylate), pKa3 6.0 (2nd phosphonic), and pKa4 11.0 (amine).


    Output (double, KocpH);

}

void organiccarbonwaterpartitioning::reset() {

    KocpH = 0.;

}

void organiccarbonwaterpartitioning::update() {

    KocpH = Koc * std::pow(10, -pH)/(std::pow(10, -pH)+ std::pow(10, -pKa));

}

} //namespace

