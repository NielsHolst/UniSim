/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "VolatilizationPlantLeaf.h"
#include <cmath>

using namespace std;

using namespace UniSim;

namespace insecticide {
	
VolatilizationPlantLeaf::VolatilizationPlantLeaf(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<double>(Name(VP), 5.73e-8, "vapor pressure of the pesticide (Pa) at 25 deg C");
    addVariable<double>(Name(ln_ER), "evaporation rate in ug m-2 h-1");
    addVariable<double>(Name(kTr), "rate constant of vaporization at reference temperature, (h-1) at 25 deg C");
    addVariable<double>(Name(kTa), "rate constant of vaporization at actual temperature, (h-1)");
    addParameter<double>(Name(Ea), 95., "activation energy in KJ mol-1");
    addParameter<double>(Name(R), 8.3145, "gas constant in J K-1 mol-1");
    addParameter<double>(Name(Tr), 304, "reference temperature in K");
    addParameter<double>(Name(Ta), 299, "actual temperature in K");
    addVariable<double>(Name(flv), "fraction lost from leaves(unit?)");
    addParameter<double>(Name(dosage), 360., "actual dose applied (ug m-2)");
    addParameter<double>(Name(inflow), 0., "inflow of susbtance per day");
    addParameter<double>(Name(t), 24., "contact time in hours");
    addVariable<double>(Name(outflow), "Current outflow of substance per day");
    addVariable<double>(Name(concentration), "Current outflow of substance per day");

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

    double ln_ER = 12.2 + 0.933 * log(VP);
    kTr = exp(ln_ER)/dosage;
    outflow = inflow * flv;
    concentration += inflow - outflow;
    flv = exp (- kTa * t);
    kTa = kTr * exp(Ea/R*(1/Tr - 1/Ta));



}

} //namespace

