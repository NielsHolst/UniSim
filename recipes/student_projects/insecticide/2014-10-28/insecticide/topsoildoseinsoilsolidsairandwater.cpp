/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "topsoildoseinsoilsolidsairandwater.h"
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (topsoildoseinsoilsolidsairandwater)
	
topsoildoseinsoilsolidsairandwater::topsoildoseinsoilsolidsairandwater(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, Za, 0.);           //mol/m3Pa
    Input (double, Zw, 0.);           //mol/m3Pa
    Input (double, Zs, 0.);
    Input (double, Vs, 0.);           //volume of solids in the soil volume susceptible for volatilization (m3)
    Input (double, Vw, 0.);           //volume of water in the soil volume susceptible for volatilization (m3)
    Input (double, Va, 0.);           //volume of air in the soil volume susceptible for volatilization (m3)
    Input (double, VZ, 0.);
    Input (double, Dosers, 0.);       //dose of the applied pesticide reaching the soil (kg ha-1)

    Output (double, Doserss);
    Output (double, Dosersw);
    Output (double, frsw);
    Output (double, Dosersa);
}

void topsoildoseinsoilsolidsairandwater::reset() {

    Doserss = 0.;
    Dosersw = 0.;
    frsw = 0.;
    Dosersa = 0.;
}


void topsoildoseinsoilsolidsairandwater::update() {

    Doserss = Dosers * ((Vs*Zs)/VZ);      //Fraction pesticide in soil solids; ((Vs*Zs)/VZ)
    Dosersw = Dosers * ((Vw*Zw)/VZ);      //Fraction pesticide in soil water; ((Vw*Zw)/VZ)
    frsw = (Vw*Zw)/VZ;                    //Fraction pesticide in soil water
    Dosersa = Dosers * ((Va*Za)/VZ);      //Fraction pesticide in soil air; ((Va*Za)/VZ)

}

} //namespace

