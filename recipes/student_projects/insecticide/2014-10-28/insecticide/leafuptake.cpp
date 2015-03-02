/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "leafuptake.h"
#include <cmath>
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (leafuptake)
	
leafuptake::leafuptake(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, a, 0.);
    Input (double, b, 0.);
    Input (double, R, 8.3145);          //gas constant
    Input (double, T, 25.);             //deg C
    Input (double, MV, 140.83);         //cm3/mol
    Input (double, leaftype, 1);

    Output (double, Ed);
    Output (double, klu);               //rate constant of the uptake process, h-1
    Output (double, log_klu);           //log10(k)
    Output (double, V);
}

void leafuptake::reset() {

    klu = 0.;
    log_klu = 0.;    
    V = 0.;
    Ed = 0.;
}

void leafuptake::update() {

    V = (0.9445 * MV) + 2.7717;
    //leaftype; citrus = 1, pear = 2 (model will assume its pear)
    Ed = (leaftype == 1) ? 78.875 * exp(3.470*MV/1000.) : 63.222 * exp(2.874*MV/1000.);
    if(leaftype == 1)
        a = 0.8, b = -0.1167;
    else
        a = 3.231, b = 6.341e-2;
    log_klu  = a - b*V - (Ed*1000)/(R*2.3*(T + 273.15));
    klu = std::pow(10, log_klu)*3600.;

}

} //namespace

