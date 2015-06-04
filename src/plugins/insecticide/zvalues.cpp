/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "zvalues.h"
#include <cmath>
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (zvalues)
	
zvalues::zvalues(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, MW, 169.07);         //molar mass of the insecticide in g/mol
    Input (double, Pd, 0.01);           //penetration depth of 1 cm = 0.01 m
    Input (double, VP, 1.31e-5);        //Pa at 25 degrees celsius
    Input (double, Psol, 12.);          //solubility g/L or kg/m3
    Input (double, foc, 0.19);
    Input (double, pb, 1.1);          // 1.1 kg/L which is equivalent to 1.1 g/cm3 or 1100 kg/m3
    Input (double, fa, 0.25);           //fraction of air in the soil
    Input (double, fw, 0.25);           //fraction of water in the soil
    Input (double, fs, 0.5);            //fraction of solids in the soil
    Input (double, KocpH, 5.678e-6);    //The acid dissociation constants for glyphosate are pKa1 0.8 (1st phosphonic), pKa2 2.3 (carboxylate), pKa3 6.0 (2nd phosphonic), and pKa4 11.0 (amine).
    Input (double, TrefVP, 25.);        //reference temperature of vapour pressure (degrees celsius)
    Input (double, Tsa, 30.);           //Average soil air temperature in the month of pesticide application  (degrees celsius)
    Input (double, R, 8.3145);          //J mol-1 K-1
    Input (double, Sp, 0.5);            //soil porosity
    Input (double, farea, 100.);


    Output (double, Za);                //mol m-3 Pa-1
    Output (double, Zw);                //mol m-3 Pa-1
    Output (double, Zs);                //mol m-3 Pa-1
    Output (double, VPTadap);
    Output (double, Vs);
    Output (double, Vw);
    Output (double, Va);
    Output (double, VZ);

}

void zvalues::reset() {
    Za = 0.;
    Zw = 0.;
    Zs = 0.;
    Vs = 0.;
    Vw = 0.;
    Va = 0.;
    VPTadap = 0.;
    VZ = 0.;

}

void zvalues::update() {

    VPTadap = VP * exp((1./10.)*(Tsa - TrefVP));
    Zw = 1000.*Psol/(VPTadap * MW);                 //Psol in g/L, MW in g/mol
    Za = 1./(R*(Tsa + 273.15));                   //za is in mol/m3Pa
    Zs = (KocpH *foc * pb * Zw) / (1. - Sp);
    Vs = Pd*fs*farea;               //volume of solids in the soil volume susceptible for volatilization (m3)
    Vw = Pd*fw*farea;               //volume of water in the soil volume susceptible for volatilization (m3)
    Va = Pd*fa*farea;               //volume of air in the soil volume susceptible for volatilization (m3)
    VZ = Va*Za + Vw*Zw + Vs*Zs;

    //vaa = Va/(Vs + Vw + Va);
    //vww = Vw/(Vs + Vw + Va);

}

} //namespace

