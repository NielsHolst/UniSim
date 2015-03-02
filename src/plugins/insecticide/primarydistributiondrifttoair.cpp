/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "primarydistributiondrifttoair.h"
#include <cmath>
#include "publish.h"


using namespace UniSim;

namespace insecticide {

PUBLISH (primarydistributiondrifttoair)
	
primarydistributiondrifttoair::primarydistributiondrifttoair(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, Nd, 0.1);    //meter (m)
    Input (double, W, 100.);     //m
    Input (double, Pnsz, 0.);       //m
    Input (double, Doseappl, 1.);    //kg/ha
    Input (double, A0, 39.);
    Input (double, A1, 0.90);
    Input (double, B0, 2.28);
    Input (double, B1, 0.147);

    Output (double, Xpass);     //m
    Output (double, fXpass);
    Output (double, Dosedrift);       //kg/ha
    Output (double, fd);

}

void primarydistributiondrifttoair::reset() {

    fXpass = 0.;
    Xpass = 0.;
    Dosedrift = 0.;
    fd = 0.;

}


void primarydistributiondrifttoair::update() {    

    int npass = int((W -(2.*Pnsz))/Nd);     //unitless
    fXpass = 0;
    for(int i = 1; i <= npass; ++i){
        Xpass = Pnsz + (Nd * (i - 0.5));    //"i" is the "pass"
        double x = A0*exp(-Xpass/B0) + A1*exp(-Xpass/B1);   //page #13 of IMAG calculator manual
        if (x > 1)
            fXpass += 1;
        else if   (x < 0)
            fXpass += 0;
        else
            fXpass += x;
    }
    fd = fXpass/npass;
    Dosedrift = Doseappl * fd;
}

} //namespace

