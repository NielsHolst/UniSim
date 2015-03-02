/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
//#include <usbase/exception.h>
#include "leafsumofdoseremoved.h"
#include <cmath>
#include "publish.h"


using namespace UniSim;

namespace insecticide {

PUBLISH (leafsumofdoseremoved)
	
leafsumofdoseremoved::leafsumofdoseremoved(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, P, 25.);             //rainfall
    Input (double, threshold, 1.);      //rainfall >1 mm
    Input (double, inflow, 0.);
    Input (double, kov, 0.);

    Output (double, outflow);
    Output (double, concentration);
    Output (double, Doselsodr);
    //sum of the 3 leaf removal processes, or
    //dose of the pesticides reaching the leaves that are removed from the leaves before the next rain event

}

void leafsumofdoseremoved::reset() {

    Doselsodr = 0.;
    outflow = 0.;
    concentration = 0.;
}

void leafsumofdoseremoved::update() {    

    if (P <= threshold){
        outflow = concentration*kov;
        concentration += inflow - outflow;
        Doselsodr += outflow;
    }
    else {
        concentration = 0;
        Doselsodr = 0;
    }

}

} //namespace

