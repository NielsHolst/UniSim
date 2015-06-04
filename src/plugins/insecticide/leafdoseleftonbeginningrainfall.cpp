/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
//#include <usbase/exception.h>
#include "leafdoseleftonbeginningrainfall.h"
#include <cmath>
#include "publish.h"


using namespace UniSim;

namespace insecticide {

PUBLISH (leafdoseleftonbeginningrainfall)
	
leafdoseleftonbeginningrainfall::leafdoseleftonbeginningrainfall(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, kov, 0.);       //h-1
    Input (double, Rainfall, 25.);          //rainfall (mm)
    Input (double, threshold, 1.);  // 1 mm
    Input (double, inflow, 0.);     // = Doserl (kg a.i/ha)

    Output (double, Doseldlobr);    //of pesticide applied left on the leaves when the next rainfall event starts (kg a.i/ha)
    Output (double, outflow); //kg a.i/ha
    Output (double, concentration); //kg a.i/ha
}

void leafdoseleftonbeginningrainfall::reset() {

    Doseldlobr = 0.;
    outflow = 0.;
    concentration = 0.;
}

void leafdoseleftonbeginningrainfall::update() {

    if (Rainfall <= threshold){
        outflow = concentration*kov;
        concentration += inflow - outflow;
        Doseldlobr = 0.;

    }
    else {
        Doseldlobr = concentration;
        concentration = 0.;
    }

}

} //namespace

