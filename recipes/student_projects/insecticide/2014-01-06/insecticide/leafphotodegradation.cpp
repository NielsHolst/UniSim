/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "leafphotodegradation.h"
#include "publish.h"
#include <cmath>

using namespace UniSim;

namespace insecticide {

PUBLISH (leafphotodegradation)
	
leafphotodegradation::leafphotodegradation(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, inflow, 0.);
    Input (double, I, 0.);
    Input (double, kOH, 0.);
    Input (double, train, 0.);
    Input (double, dl, 0.);
    Output (double, leafoxidationrateconstant);
    Output (double, concentration);
    Output (double, outflow);
    Output (double, concentrationofOHradical);
}

void leafphotodegradation::reset() {
    concentration = 0.;
    outflow = 0.;
    concentrationofOHradical = 0.;
}

void leafphotodegradation::update() {

    leafoxidationrateconstant = kOH * pow (10, 3.994e-3 * I + 4.158);
    outflow = inflow * leafoxidationrateconstant*train*dl/24;
    //outflow = inflow * exp(-leafoxidationrateconstant*train*dl/24); //this equation produces similar output as the above equation
    //outflow = concentration * leafoxidationrateconstant*train*dl/24;
    //outflow = concentration * exp(-leafoxidationrateconstant*train*dl/24);

    //outflow = leafoxidationrateconstant*concentration;
    //concentration += inflow - outflow;

}

} //namespace

