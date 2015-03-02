/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
//#include <usbase/exception.h>
#include "leaffinaldosephotodegraded.h"
#include <cmath>
#include "publish.h"


using namespace UniSim;

namespace insecticide {

PUBLISH (leaffinaldosephotodegraded)
	
leaffinaldosephotodegraded::leaffinaldosephotodegraded(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, kld, 0.);        //h-1
    Input (double, kov, 0.);        //h-1
    Input (double, P, 25.);
    Input (double, inflow, 0.);
    Input (double, threshold, 1.);

    Output (double, Doselfdpd);     //final dose removed by volatilization
    Output (double, outflow);
    Output (double, concentration);

}

void leaffinaldosephotodegraded::reset() {

    Doselfdpd = 0.;
    outflow = 0.;
    concentration = 0.;
}

void leaffinaldosephotodegraded::update() {

    if (P <= threshold){
        outflow = concentration*(kld/kov);
        concentration += inflow - outflow;
        Doselfdpd += outflow;
    }
    else {
        concentration = 0;
        Doselfdpd = 0;
    }

}

} //namespace

