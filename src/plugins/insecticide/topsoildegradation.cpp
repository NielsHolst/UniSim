/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "topsoildegradation.h"
#include <cmath>
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (topsoildegradation)
	
topsoildegradation::topsoildegradation(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, inflow, 0.);
    Input (double, k, 0.);          //biodegradation rate constant (h-1)
    Input (double, ff, 0.995);      //fractionformation of a metabolite
    Input (double, Tref, 25.);      //Reference temperature at which t½ has been determined  in deg C
    Input (double, Sp, 0.);         //soil porosity
    Input (double, Q, 2.1);         //value to calculate the temperature correction factor for soil biodegradation rate (this value ranges from 2-3, in this model the value of 2.1 is adopted)
    Input (double, Tair, 25.);    
    Input (double, Vrsmc, 0.5);     //reference soil moisture content
    Input (double, rf, 0.7);        //exponential response factor for the influence of moisture content on biodegradation rate
    Input (double, fw, 0.25);       //fraction of water in the soil

    Output (double, concentration);
    Output (double, outflow);    
    Output (double, Ts);             //actual soil temp in deg C
    Output (double, fsdT);           //temperature correction factor
    Output (double, Vsmc);           //moisture content
    Output (double, fsm);            //moisture correction factor
    Output (double, ksd);            //moisture & temperature corrected biodegradation rate constant (h-1)
}

void topsoildegradation::reset() {
    concentration = 0.;
    outflow = 0.;

    Ts = 0.;
    ksd = 0.;                        // k = ln(2)/t(1/2)
    fsdT = 0.;
    Vsmc = 0.;
    fsm = 0.;
}

void topsoildegradation::update() {
    Ts = 1.05*Tair - 1.5;
    fsdT = std::pow(Q,(Ts - Tref)/10.);
    Vsmc = (Sp > 0) ? fw/Sp : 0;
    fsm = (Vrsmc > 0) ? std::pow((Vsmc/Vrsmc),rf) : 0;
    ksd = k*fsdT*fsm;
    outflow = concentration*ff*ksd;
    concentration += inflow - outflow;
}

} //namespace

