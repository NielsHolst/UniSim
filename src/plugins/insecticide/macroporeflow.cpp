/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "macroporeflow.h"
#include <cmath>
#include "publish.h"



using namespace UniSim;

namespace insecticide {

PUBLISH (macroporeflow)
	
macroporeflow::macroporeflow(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, fsand, 0.25);
    Input (double, fsilt, 0.25);
    Input (double, fclay, 0.5);
    Input (double, fa, 0.25);           //fraction of air in the soil
    Input (double, fw, 0.25);           //fraction of water in the soil
    Input (double, fmacropore, 0.3);    //fraction of macropore i.e. pore fractions which acts as macropores
    Input (double, Flowrate, 0.35);
    Input (double, frr, 0.5);           //fractionrainrunoff
    Input (double, tpevent, 5.);        //number of hours precipitation occurs on a rainy day
    Input (double, P, 25.);
    Input (double, frsw, 0.);
    Input (double, Tf, 0.);            //tillage factor
    Input (double, Doseaflm, 0.);

    Output (double, fporemobile);       //mobile pore fraction in topsoil
    Output (double, Vporeimmobile);     //immobile pore volume in topsoil
    Output (double, C);                 //one hour storage capacity
    Output (double, Cused);             //used capacity
    Output (double, Cfree);
    Output (double, Pcapacitymm);       //mm rain capacity or freecapacity recalculated as the number of mm rain that can be taken up in 1 hour (mm)
    Output (double, Preqh);             //rainfall required for macropore flow per hour
    Output (double, Preqday);           //rainfall required for macropore flow per day
    Output (double, Pi);                //rainfall distribution integrated
    Output (double, Dmacropore);
    Output (double, Vporemobile);       //mobile pore volume in topsoil

}

void macroporeflow::reset() {
    fporemobile = 0.;
    Vporemobile = 0.;
    Vporeimmobile = 0.;
    C = 0.;
    Cused = 0.;
    Cfree = 0.;
    Pcapacitymm = 0.;
    Preqh = 0.;
    Preqday = 0.;
    Pi = 0.;
    Dmacropore = 0.;

}

void macroporeflow::update() {

    fporemobile = 0.72 * fsand + 0.35 * fsilt + 0.14 * fclay;
    Vporemobile = fporemobile * (fw * fa);
    Vporeimmobile = (fw + fa)- Vporemobile;
    C = Vporeimmobile * 0.01 + ((1. - fmacropore) * Vporemobile *Flowrate/24.);
    Cused = (fw /(fw + fa))* C;
    Cfree = C - Cused;
    Pcapacitymm = 1000. * Cfree;
    Preqh = Pcapacitymm /(1. - frr);
    Preqday = Preqh * 24.;
    Pi = 1.-(1.- exp((-tpevent /(24.*P)*Preqday)));
    Dmacropore = Pi*frsw*Doseaflm*(Tf/7.5);
}

} //namespace

