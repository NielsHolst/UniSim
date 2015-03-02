/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "topsoilvolatilizationfinal.h"
#include <cmath>
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (topsoilvolatilizationfinal)
	
topsoilvolatilizationfinal::topsoilvolatilizationfinal(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, ksv, 0.0001);       //volatilization rate constant (h-1)    
    Input (double, P, 25.);            //rainfall (mm)
    Input (double, threshold, 1.);     //rainfall even greater than 1 mm
    Input (double, inflow, 0.);        //Dosers
    Input (double, Va, 0.);
    Input (double, Za, 0.);
    Input (double, Vw, 0.);
    Input (double, Zw, 0.);
    Input (double, VZ, 0.);

    Output (double, fv);               //fraction of pesticide susceptible for vaporization
    Output (double, concentration);    //svDoseaflrm
    Output (double, outflow);          //Dosevap,s
    Output (double, frsw);             //fraction of pesticide in soil water
    Output (double, frsa);             //fraction of pesticide in soil air
    Output (double, svDoseaflrm);      //dose remained at the next rainfall event is taken for leaching
    Output (double, Dosetsv);          //dose removed by vaporization from toposoil

}

void topsoilvolatilizationfinal::reset() {

    fv = 0.;
    concentration = 0.;
    outflow = 0.;
    frsa = 0.;
    frsw = 0.;
    svDoseaflrm = 0.;
    Dosetsv = 0.;
}

void topsoilvolatilizationfinal::update() {

    frsa = Va*Za/VZ;
    frsw = Vw*Zw/VZ;
    fv = frsa + frsw;

    if (P <= threshold){
        outflow = concentration*ksv;
        //outflow = concentration*fv*ksv;
        concentration += inflow - outflow;
        svDoseaflrm = 0.;
        Dosetsv +=outflow;
    }
    else {
        svDoseaflrm = concentration;
        concentration = 0.;
        Dosetsv = 0;
    }

}

} //namespace

