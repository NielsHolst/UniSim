/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "crop_biomass.h"
#include "publish.h"

using namespace std;
using namespace UniSim;

namespace vg {

PUBLISH(CropBiomass)

/*! \class CropBiomass
 * \brief Keeps track of crop biomass and energy budget
 *
 * Inputs
 * ------
 * - _Tcrop_ is crop temperature [<SUP>o</SUP>C]
 * - _Pgc_ is crop assimilation rate [g CO<SUB>2</SUB>/m<SUP>2</SUP>/h]
 * - _timeStep_ is the integration time step [s]
 * - _initDryWeight_ is the initial dry weight of the crop [g/m<SUP>2</SUP>]
 *
 * Output
 * ------
 * - _maintenanceRespiration_ is the biomass lost to maintenance during one time step [g/m<SUP>2</SUP>]
 * - _conversionCost_ is the biomass used for conversion during one time step [g/m<SUP>2</SUP>]
 * - _dDryWeight_ is the increment in crop biomass during one time step [g/m<SUP>2</SUP>]
 * - _dryWeight_ is the crop biomass [g/m<SUP>2</SUP>]
 *
 * Default dependencies
 * ------------
 * - a _temperature_ sibling model with a _value_ port [<SUP>o</SUP>C]
 * - an _Pgc_ child model with a _value_ port [g CO<SUB>2</SUB> /m<SUP>2</SUP>/h]
 * - an _calendar_ model with an _timeStepSecs_ port [s]
 */

CropBiomass::CropBiomass(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, Tcrop, "../temperature[value]");
    InputRef(double, Pgc, "./Pgc[value]");
    InputRef(double, timeStep, "calendar[timeStepSecs]");
    Input(double, initDryWeight, 10);

    Output(double, maintenanceRespiration);
    Output(double, conversionCost);
    Output(double, dDryWeight);
    Output(double, dryWeight);
}

void CropBiomass::reset() {
    dryWeight = initDryWeight;
}

void CropBiomass::update() {
    const double Q10 = 2.;
    double dt = timeStep/3600; // [h]
    maintenanceRespiration = maintenanceRespirationRate()*dryWeight*dt*pow(Q10, 0.1*(Tcrop-25.));
    double forConstruction = max(Pgc*dt*30./44. - maintenanceRespiration, 0.);
    dDryWeight = forConstruction/conversionEfficiency();
    dryWeight += dDryWeight;
    conversionCost = forConstruction - dDryWeight;
}

double CropBiomass::maintenanceRespirationRate() {
    // At 25 oC [g CH2O/g/d]
    const double leaves = 0.015,
                 stem = 0.03,
                 fruits = 0.01;
    // At 25 oC [g CH2O/g/h]
    return (0.7*leaves + 0.25*stem + 0.05*fruits)/24.;
}

double CropBiomass::conversionEfficiency() {
    const double leaves = 1.39,
                 stem = 1.45,
                 root = 1.39,
                 fruits = 1.35;
    return (leaves + stem + root + fruits)/4.;
}


} //namespace

