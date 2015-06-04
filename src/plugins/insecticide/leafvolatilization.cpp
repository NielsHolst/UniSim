/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "leafvolatilization.h"
#include <cmath>
#include "publish.h"

using namespace std;

using namespace UniSim;

namespace insecticide {
	
PUBLISH(leafvolatilization)

leafvolatilization::leafvolatilization(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, VP, 5.73e-8);    //vapor pressure of the pesticide (Pa) at 25 degC
    Input (double, Ea, 95.);        //activation energy in kJ/mol (default value)
    Input (double, R, 8.3145);      //gas constant in J K-1 mol-1
    Input (double, Tr, 25.);        //reference temperature in degC
    Input (double, Ta, 25.);        //air temperature at the time of pesticide application, degC
    Input (double, Doseappl, 3.6);  //application rate (kg/ha)automatically converted to ug/m2

    Output (double, ln_ER);         //evaporation rate in ug m-2 h-1 or microgram/m2/h
    Output (double, kTr);           //rate constant of vaporization at reference temperature, (h-1) at 25 deg C
    Output (double, ER);            //evaporation rate (microgram/m2/h)
    Output (double, fv);            //fraction of pesticide emitted in 1 h
    Output (double, fr);            //Fraction remaining on leaf -or-fraction of pesticide reaching the leave still there in 1 hour
    Output (double, klv);           //rate constant of vaporization at actual temperature, (h-1)

}

void leafvolatilization::reset() {

    ln_ER = 0.;
    ER = 0.;
    kTr = 0.;
    klv = 0.;
    fv = 0.;
    fr = 0.;

}

void leafvolatilization::update() {

    double x = (VP > 0) ? log(VP) : 0;
    ln_ER = 12.2 + 0.933 * x;          //in C++ log is "ln", and log10 is "log"
    ER = exp(ln_ER);
    fv = (ER/(Doseappl*(1e9/1e4)) <1) ? ER/(Doseappl*(1e9/1e4)) : 0.99;
    fr = 1 - fv;
    kTr = -log(fr);
    klv = kTr * exp(((Ea*1000.)/R)*((1./(Tr + 273.15)) - (1./(Ta + 273.14))));

  }

} //namespace

