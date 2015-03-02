/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
//#include <usbase/exception.h>
#include "leafdosevolatilized.h"
#include <cmath>
#include "publish.h"


using namespace UniSim;

namespace insecticide {

PUBLISH (leafdosevolatilized)
	
leafdosevolatilized::leafdosevolatilized(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, klv, 0.);        //rate constant of volatilization process (h-1)
    Input (double, inflow, 0.);
    Input (double, threshold, 1.);  //rainfall >1 mm
    Input (double, P, 0.);          //rainfall (mm)

    Output (double, Doseldv);
    Output (double, outflow);
    Output (double, concentration);

}

void leafdosevolatilized::reset() {

    Doseldv = 0.;
    outflow = 0.;
    concentration = 0.;
}

void leafdosevolatilized::update() {

    if (P <= threshold){
        outflow = concentration*klv;
        concentration += inflow - outflow;
        Doseldv += outflow;

    }
    else {
        concentration = 0;
        Doseldv = 0;
    }

}

} //namespace

