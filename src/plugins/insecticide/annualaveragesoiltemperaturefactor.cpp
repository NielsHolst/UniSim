/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "annualaveragesoiltemperaturefactor.h"
#include <cmath>
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (annualaveragesoiltemperaturefactor)
	
annualaveragesoiltemperaturefactor::annualaveragesoiltemperaturefactor(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, Tref, 25.);              //Reference temperature at which t½ has been determined  in deg C
    Input (double, Q, 2.1);                 //value to calculate the temperature correction factor for soil biodegradation rate (this value ranges from 2-3, in this model the value of 2.1 is adopted)
    Input (double, Tsann, 30.);     //Annual average soil temperature in deg C

    Output (double, FssdT);      //factor to correct for soil temperature, based on averaged annual soil temperature

}

void annualaveragesoiltemperaturefactor::reset() {

    FssdT = 0.;
}

void annualaveragesoiltemperaturefactor::update() {

    FssdT = std::pow(Q,(((1.05 * Tsann) + 1.5)/Tref)/10.);

}

} //namespace

