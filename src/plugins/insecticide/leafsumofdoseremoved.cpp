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

    Input (double, Rainfall, 25.);      //rainfall
    Input (double, threshold, 1.);      //rainfall >1 mm
    Input (double, inflow, 0.); //kg a.i/ha
    Input (double, kov, 0.); // per hour (h-1)

    Output (double, outflow); //kg a.i/ha
    Output (double, concentration); //kg a.i/ha
    Output (double, Doselsodr); //kg a.i/ha
    //sum of the 3 leaf removal processes, or
    //dose of the pesticides reaching the leaves that are removed from the leaves before the next rain event
}

void leafsumofdoseremoved::reset() {

    Doselsodr = 0.;
    outflow = 0.;
    concentration = 0.;
}

void leafsumofdoseremoved::update() {

        if (Rainfall <= threshold){
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

