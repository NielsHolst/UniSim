/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "fractionofrainrunoff.h"
#include "publish.h"


using namespace UniSim;

namespace insecticide {

PUBLISH (fractionofrainrunoff)
	
fractionofrainrunoff::fractionofrainrunoff(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, Q, 5.);
    Input (double, P, 25.);


    Output (double, frr);

}

void fractionofrainrunoff::reset() {

    frr = 0.;

}

void fractionofrainrunoff::update() {

    frr = (P > 0) ? Q/P : 0;

}

} //namespace

