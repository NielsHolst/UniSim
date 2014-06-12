/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "VolatilizationPlantLeaf.h"
#include "publish.h"
#include <cmath>

using namespace std;

using namespace UniSim;

namespace insecticide {
	
PUBLISH(VolatilizationPlantLeaf)

VolatilizationPlantLeaf::VolatilizationPlantLeaf(Identifier name, QObject *parent)
	: Model(name, parent)
{
//    addParameter<double>(Name(VP), 5.73e-8, "vapor pressure of the pesticide (Pa) at 25 deg C");
//    addVariable<double>(Name(ln_ER), "evaporation rate in ug m-2 h-1");
//    addVariable<double>(Name(kTr), "rate constant of vaporization at reference temperature, (h-1) at 25 deg C");
//    addVariable<double>(Name(kTa), "rate constant of vaporization at actual temperature, (h-1)");
//    addParameter<double>(Name(Ea), 95., "activation energy in KJ mol-1");
//    addParameter<double>(Name(R), 8.3145, "gas constant in J K-1 mol-1");
//    addParameter<double>(Name(Tr), 304, "reference temperature in K");
//    addParameter<double>(Name(Ta), 299, "actual temperature in K");
//    addVariable<double>(Name(flv), "fraction lost from leaves(unit?)");
//    addParameter<double>(Name(dosage), 360., "actual dose applied (ug m-2)");
//    addParameter<double>(Name(inflow), 0., "inflow of susbtance per day");
//    addParameter<double>(Name(t), 24., "contact time in hours");
//    addVariable<double>(Name(outflow), "Current outflow of substance per day");
//    addVariable<double>(Name(concentration), "Current outflow of substance per day");
    Input (double, VP, 5.73e-8);
    Output (double, ln_ER);
    Output (double, kTr);
    Output (double, kTa);
    Input (double, Ea, 95.);
    Input (double, R, 8.3145);
    Input (double, Tr, 304);
    Input (double, Ta, 299);
    Output (double, flv);
    Input (double, dosage, 360.);
    Input (double, inflow, 0.);
    Input (double, t, 24.);
    Output (double, outflow);
    Output (double, concentration);
}


void VolatilizationPlantLeaf::reset() {

    ln_ER = 0.;
    kTr = 0.;
    kTa = 0.;
    flv = 0.;
    outflow = 0.;
    concentration = 0.;

}

void VolatilizationPlantLeaf::update() {

    // for loop execution

          for( int i = 1; i < 121; ++i ){

            double ln_ER = 12.2 + 0.933 * log(VP);
            kTr = exp(ln_ER)/dosage;
            kTa = kTr * exp(Ea/R*(1/Tr - 1/Ta));
            flv = exp (- kTa * t);
            outflow = inflow * flv;
            inflow=concentration;
            concentration = inflow - outflow;

           }

}

} //namespace

