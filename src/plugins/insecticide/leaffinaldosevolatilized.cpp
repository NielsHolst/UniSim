/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
//#include <usbase/exception.h>
#include "leaffinaldosevolatilized.h"
#include <cmath>
#include "publish.h"


using namespace UniSim;

namespace insecticide {

PUBLISH (leaffinaldosevolatilized)
	
leaffinaldosevolatilized::leaffinaldosevolatilized(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, klv, 0.);        //h-1
    Input (double, kov, 0.);        //h-1
    Input (double, threshold, 1.); //rainfall (mm)
    Input (double, P, 25.); //rainfall (mm)
    Input (double, inflow, 0.); //kg a.i/ha

    Output (double, Doselfdv);      //final dose removed by volatilization
    Output (double, concentration); //kg a.i/ha
    Output (double, outflow); //kg a.i/ha

}

void leaffinaldosevolatilized::reset() {

    Doselfdv = 0.;
    concentration = 0.;
    outflow = 0.;
}

void leaffinaldosevolatilized::update() {    

    if (P <= threshold){
        outflow = concentration*(klv/kov);
        concentration += inflow - outflow;
        Doselfdv += outflow;
    }
    else {
        concentration = 0;
        Doselfdv = 0;
    }

}

} //namespace

