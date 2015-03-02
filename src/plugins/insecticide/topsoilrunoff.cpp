/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "topsoilrunoff.h"
#include <cmath>
#include "publish.h"


using namespace UniSim;

namespace insecticide {

PUBLISH (topsoilrunoff)
	
topsoilrunoff::topsoilrunoff(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, S, 0.01);
    Input (double, wbz, 3.3);
    Input (double, Doseaflrm, 0.);
    Input (double, Doseldlobr, 0.);     //dose leaft on the leaf on beginning rainfall will be washed to the soil
    Input (double, P, 25.);
    Input (double, KocpH, 5.678e-6);
    Input (double, fom, 0.09);          //organice matter content in the soil
    Input (double, Q, 5.);

    Output (double, F);
    Output (double, Fslope);
    Output (double, Fbuffer);
    Output (double, Dosesr);
}

void topsoilrunoff::reset() {
    Fslope = 0.;
    Fbuffer = 0.;
    F = 0.;
    Dosesr = 0.;
    Kd = 0.;

}

void topsoilrunoff::update() {

    if (S > 0.2){
        Fslope = 1;
    }
    else if (S <0.2){
        Fslope = 0.02153*S + 0.001423*std::pow(S, 2);
    }
    else{
        Fslope = 0.02153 + 0.0014426;
    }
    Fbuffer = std::pow(0.83, wbz);
    F = Fbuffer * Fslope;
    Kd = KocpH * fom;
    Dosesr = (P > 0) ? (Doseaflrm + Doseldlobr) *(Q/P) * F * (100./ 1. + Kd) : 0;

}

} //namespace

