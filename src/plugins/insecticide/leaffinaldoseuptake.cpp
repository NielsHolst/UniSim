/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
//#include <usbase/exception.h>
#include "leaffinaldoseuptake.h"
#include <cmath>
#include "publish.h"


using namespace UniSim;

namespace insecticide {

PUBLISH (leaffinaldoseuptake)
	
leaffinaldoseuptake::leaffinaldoseuptake(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, klu, 0.);                   //h-1
    Input (double, kov, 0.);        //h-1
    Input (double, P, 25.);  // rainfall (m)
    Input (double, inflow, 0.); //kg a.i/ha
    Input (double, threshold, 1.); //rainfall (mm)

    Output (double, Doselfdu);      //final dose removed by uptake process
    Output (double, outflow); //kg a.i/ha
    Output (double, concentration); //kg a.i/ha

}

void leaffinaldoseuptake::reset() {

    Doselfdu = 0.;
    outflow = 0.;
    concentration = 0.;
}

void leaffinaldoseuptake::update() {

    if (P <= threshold){
        outflow = concentration*(klu/kov);
        concentration += inflow - outflow;
        Doselfdu += outflow;
    }
    else {
        concentration = 0;
        Doselfdu = 0;
    }
}
} //namespace

