/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
//#include <usbase/exception.h>
#include "leafdoseuptaken.h"
#include <cmath>
#include "publish.h"


using namespace UniSim;

namespace insecticide {

PUBLISH (leafdoseuptaken)
	
leafdoseuptaken::leafdoseuptaken(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, klu, 0.);        //rate constant of volatilization process (h-1)
    Input (double, inflow, 0.); //kg a.i/ha
    Input (double, threshold, 1.);  //rainfall >1 mm
    Input (double, Rainfall, 0.);          //rainfall (mm)

    Output (double, Doseldu); //kg a.i/ha
    Output (double, outflow); //kg a.i/ha
    Output (double, concentration); //kg a.i/ha

}

void leafdoseuptaken::reset() {

    Doseldu = 0.;
    outflow = 0.;
    concentration = 0.;
}

void leafdoseuptaken::update() {

    if (Rainfall <= threshold){
        outflow = concentration*klu;
        concentration += inflow - outflow;
        Doseldu += outflow;

    }
    else {
        concentration = 0;
        Doseldu = 0;
    }

}

} //namespace

