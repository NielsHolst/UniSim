/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "topsoilvolatilization.h"
#include <cmath>
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (topsoilvolatilization)
	
topsoilvolatilization::topsoilvolatilization(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, Tsa, 25.);
    Input (double, MWair, 28.97);       // g/mol
    Input (double, MVair, 20.1);        // Molar volume air; cm3/mol
    Input (double, MV, 140.83);         //molar volume; density 1.704 g/cm3, molar mass 169.07 g/mol
    Input (double, BLair, 0.00475);     // m or 4.75 mm as per Mackay (2001)
    Input (double, Pd, 0.01);           //penetration depth of 1 cm = 0.01 m
    Input (double, fa, 0.25);           //fraction of air in the soil
    Input (double, fw, 0.25);           //fraction of water in the soil
    Input (double, Pair, 1.);           //atm
    Input (double, farea, 100.);        //m2
    Input (double, Za, 0.);             //mol/m3Pa
    Input (double, Zw, 0.);             //mol/m3Pa
    Input (double, VZ, 0.);
    Input (double, MW, 169.07);         // molecular weight or molar mass of the insecticide in g/mol
    Input (double, viscosity, 0.);      //viscosity of water (centipoise),between 0 - 50 deg C

    Output (double, Ba);
    Output (double, Dbl);    
    Output (double, Bea);
    Output (double, Bew);
    Output (double, Da);
    Output (double, Dw);
    Output (double, Dv);
    Output (double, ksv);               //rate constant of pesticide evaporation from the soil (h-1)
    Output (double, Bw);
    Output (double, Dpl);               //diffusion path length (m)
    Output (double, ke);                //transport velocity through boundary layer (m/s)
}

void topsoilvolatilization::reset() {

    Bw = 0.;    
    Dbl = 0.;
    Ba = 0.;
    Bea = 0.;
    Bew = 0.;
    Da = 0.;
    Dw = 0.;
    Dv = 0.;
    ksv = 0.;
    Dpl = 0.;
    ke = 0.;

}

void topsoilvolatilization::update() {

    Ba = 1./10000. * 1./1000. * (std::pow((Tsa + 273.15), 1.75)*std::sqrt(((1./MWair) + (1./MW))) / (Pair*std::pow((pow(MVair, 1./3.) + std::pow(MV, 1./3.)), 2)));
    ke = Ba / BLair;
    Dbl = 3600. * ke* farea * Za;                                                   //BLT = airboundarylayer
    Bea = Ba*std::pow(fa, 10./3.)/std::pow((fa+fw), 2);
    Bw = (1./10000.)*0.0001326/(std::pow(viscosity, 1.14)* std::pow(MV, 0.589));    //molecular diffusivity of selected pesticide in water (m2/s)
    Bew = Bw*std::pow(fw, 10./3.)/std::pow((fa+fw), 2);                             //effective diffusivity (m2/s)
    Dpl = Pd / 2.;
    Da = 3600. * Bea * farea * Za / Dpl;          //mol/m3h
    Dw = 3600. * Bew * farea * Zw / Dpl;          //mol/m3h
    Dv = 1./((1./Dbl)+(1./(Da + Dw)));            //mol/m3h
    ksv = Dv/(VZ);

}

} //namespace

