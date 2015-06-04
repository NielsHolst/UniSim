/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "EaEp.h"
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (EaEp)
	
EaEp::EaEp(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, fw, 0.25);     //soil water fraction, 0.25 (default)
    Input (double, fieldcapacity, 0.);

    Output (double, ratioofactualevaporationandpotentialevaporation);

}

void EaEp::reset() {

    ratioofactualevaporationandpotentialevaporation = 0.;

    //IF(fw/fieldcapacity)<1 THEN (IF(fw/fieldcapacity)<0.8 THEN 0.15
                        //ELSE ((0.85/0.20)*((fw/fieldcapacity)-0.8))+0.15) ELSE 1;


}

void EaEp::update() {    

    double x = (fw/fieldcapacity <0.8) ? 0.15 : ((0.85/0.20)*((fw/fieldcapacity)-0.8));

    if (fw/fieldcapacity < 1){
        ratioofactualevaporationandpotentialevaporation = x + 0.15;
    }
    else{
        ratioofactualevaporationandpotentialevaporation = 1;
    }

}

} //namespace

