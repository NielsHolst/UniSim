/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "leafuptake.h"
#include "publish.h"
#include <cmath>


using namespace UniSim;

namespace insecticide {

PUBLISH (leafuptake)
	
leafuptake::leafuptake(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, inflow, 0.);
    Input (double, a, 0.);
    Input (double, b, 0.);
    Output (double, Ed);
    Input (double, R, 0.);
    Input (double, T, 0.);
    Input (double, MV, 0.);
    Output (double, log_k);
    Output (double, k);
    Output (double, concentration);
    Output (double, outflow);
    Output (double, V);
}

void leafuptake::reset() {
    concentration = 0.;
    outflow = 0.;
    log_k = 0.;
    k = 0.;
    V = 0.;
    Ed = 0.;
}

void leafuptake::update() {

    V = 0.9445*MV+2.7717;
    Ed = 78.875 * exp(3.470*MV/1000);
    log_k  = a - b*V - ((-Ed/(R*2.3*(T+273.15))))*1000; //k = rateconstantoftheuptakeprocess
    k = pow(10, log_k)*3600; //the above rate constant "k" is in hours
    outflow = k*concentration;
    concentration += inflow - outflow;
    //klu is the "rate constant of the uptake process"

}

} //namespace

