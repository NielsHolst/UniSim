/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "fieldcapacity.h"
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (fieldcapacity)
	
fieldcapacity::fieldcapacity(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, fsandLayer6, 0.);            //fsandLayer1 to fsandLayer11 are in "fractions"
    Input (double, fsandLayer2, 0.4);
    Input (double, fsandLayer3, 0.4);
    Input (double, fsandLayer10, 0.);
    Input (double, fsandLayer8, 0.);
    Input (double, fsandLayer7, 0.);
    Input (double, fsandLayer5, 0.);
    Input (double, fsandLayer1, 0.4);
    Input (double, fsandLayer4, 0.4);
    Input (double, fsandLayer9, 0.);
    Input (double, Htc3, 0.);                      //Htc1 to Htc10 have units of "m"
    Input (double, Htc2, 0.);
    Input (double, Htc4, 0.);
    Input (double, Htc1, 0.);
    Input (double, Htc5, 0.);
    Input (double, Htc6, 0.);
    Input (double, Htc7, 0.);
    Input (double, Htc8, 0.);
    Input (double, Htc9, 0.);
    Input (double, Htc10, 0.);

    Output (double, fieldcap);


}

void fieldcapacity::reset() {
    fieldcap = 0.;


}

void fieldcapacity::update() {

    fieldcap = (-0.3477*((fsandLayer1*Htc1)+(fsandLayer2*Htc2)+
               (fsandLayer3*Htc3)+(fsandLayer4*Htc4)+
               (fsandLayer5*Htc5)+(fsandLayer6*Htc6)+
               (fsandLayer7*Htc7)+(fsandLayer8*Htc8)+
               (fsandLayer9*Htc9)+(fsandLayer10*Htc10)))+0.44579;

}

} //namespace

