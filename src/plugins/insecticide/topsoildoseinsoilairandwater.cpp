/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "topsoildoseinsoilairandwater.h"
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (topsoildoseinsoilairandwater)
	
topsoildoseinsoilairandwater::topsoildoseinsoilairandwater(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, Dosersa, 0.);           //dose of pesticide in soil air
    Input (double, Dosersw, 0.);           //dose of pesticide in soil water

    Output (double, Dosesfv);           //dose of pesticide susceptible for vaporization
}

void topsoildoseinsoilairandwater::reset() {


    Dosesfv = 0.;
}


void topsoildoseinsoilairandwater::update() {


    Dosesfv = Dosersw + Dosersa;

}

} //namespace

