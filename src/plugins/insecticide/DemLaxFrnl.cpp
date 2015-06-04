/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "DemLaxFrnl.h"
#include <cmath>
#include "publish.h"


using namespace UniSim;

namespace insecticide {

PUBLISH (DemLaxFrnl)
	
DemLaxFrnl::DemLaxFrnl(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, Doseafl, 0.);            //dose of pesticides available for leaching (kg a.i/ha)
    Input (double, DLosses1, 0.);    //drainage losses per horizon, 1 to 10
    Input (double, DLosses2, 0.);
    Input (double, DLosses3, 0.);
    Input (double, DLosses4, 0.);
    Input (double, DLosses5, 0.);
    Input (double, DLosses6, 0.);
    Input (double, DLosses7, 0.);
    Input (double, DLosses8, 0.);
    Input (double, DLosses9, 0.);
    Input (double, DLosses10, 0.);
    Input (double, Fn1, 0.);
    Input (double, Fn2, 0.);
    Input (double, Fn3, 0.);
    Input (double, Fn4, 0.);
    Input (double, Fn5, 0.);
    Input (double, Fn6, 0.);
    Input (double, Fn7, 0.);
    Input (double, Fn8, 0.);
    Input (double, Fn9, 0.);
    Input (double, Fn10, 0.);

    Output (double, Dem1);
    Output (double, Dem2);
    Output (double, Dem3);
    Output (double, Dem4);
    Output (double, Dem5);
    Output (double, Dem6);
    Output (double, Dem7);
    Output (double, Dem8);
    Output (double, Dem9);
    Output (double, Dem10);
    Output (double, Drnl1);
    Output (double, Drnl2);
    Output (double, Drnl3);
    Output (double, Drnl4);
    Output (double, Drnl5);
    Output (double, Drnl6);
    Output (double, Drnl7);
    Output (double, Drnl8);
    Output (double, Drnl9);
    Output (double, Drnl10);
    Output (double, Lax1);          //dose of pesticides left after groundwater reaches soil fraction (x)
    Output (double, Lax2);
    Output (double, Lax3);
    Output (double, Lax4);
    Output (double, Lax5);
    Output (double, Lax6);
    Output (double, Lax7);
    Output (double, Lax8);
    Output (double, Lax9);
    Output (double, Lax10);
    Output (double, Subsoilfdd);    //Subsoil final dose drained, or dose of the applied pesticide that reaches drainage pipes (kg a.i/ha)
    Output (double, Subsoilfdrgw);  //dose of the pesticide applied that reaches the groundwater via leaching (kg a.i/ha)
    Output (double, Subsoilfdbd);   //dose of the pesticide applied that is lost through biodegradation in the subsoil (kg a.i/ha)
}

void DemLaxFrnl::reset() {

    Dem1 = 0.;
    Dem2 = 0.;
    Dem3 = 0.;
    Dem4 = 0.;
    Dem5 = 0.;
    Dem6 = 0.;
    Dem7 = 0.;
    Dem8 = 0.;
    Dem9 = 0.;
    Dem10 = 0.;
    Drnl1 = 0.;         //same as Frnl1
    Drnl2 = 0.;
    Drnl3 = 0.;
    Drnl4 = 0.;
    Drnl5 = 0.;
    Drnl6 = 0.;
    Drnl7 = 0.;
    Drnl8 = 0.;
    Drnl9 = 0.;
    Drnl10 = 0.;
    Lax1 = 0.;
    Lax2 = 0.;
    Lax3 = 0.;
    Lax4 = 0.;
    Lax5 = 0.;
    Lax6 = 0.;
    Lax7 = 0.;
    Lax8 = 0.;
    Lax9 = 0.;
    Lax10 = 0.;
    Subsoilfdd = 0.;
    Subsoilfdrgw = 0.;
    Subsoilfdbd = 0.;

}

void DemLaxFrnl::update() {

    Dem1 = Doseafl*DLosses1;
    Lax1 = Doseafl*1;
    Drnl1 = (Doseafl*Fn1)-(Doseafl*Dem1);

    Dem2 = Drnl1*DLosses2;
    Lax2 = Drnl1 - Dem2;
    Drnl2 = Lax2*Fn2;

    Dem3 = Drnl2*DLosses3;
    Lax3 = Drnl2 - Dem3;
    Drnl3 = Lax3*Fn3;

    Dem4 = Drnl3*DLosses4;
    Lax4 = Drnl3 - Dem4;
    Drnl4 = Lax4*Fn4;

    Dem5 = Drnl4*DLosses5;
    Lax5 = Drnl4 - Dem5;
    Drnl5 = Lax5*Fn5;

    Dem6 = Drnl5*DLosses6;
    Lax6 = Drnl5 - Dem6;
    Drnl6 = Lax6*Fn6;

    Dem7 = Drnl6*DLosses7;
    Lax7 = Drnl6 - Dem7;
    Drnl7 = Lax7*Fn7;

    Dem8 = Drnl7*DLosses8;
    Lax8 = Drnl7 - Dem8;
    Drnl8 = Lax8*Fn8;

    Dem9 = Drnl8*DLosses9;
    Lax9 = Drnl8 - Dem9;
    Drnl9 = Lax9*Fn9;

    Dem10 = Drnl9*DLosses10;
    Lax10 = Drnl9 - Dem10;
    Drnl10 = Lax10*Fn10;

    Subsoilfdd = Dem1+Dem2+Dem3+Dem4+Dem5+Dem6+Dem7+Dem8+Dem9+Dem10;

    Subsoilfdrgw = Lax10;

    Subsoilfdbd = Doseafl - (Subsoilfdd + Subsoilfdrgw);

}

} //namespace

