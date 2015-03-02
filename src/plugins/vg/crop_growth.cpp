/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "crop_growth.h"
#include "publish.h"

using namespace std;
using namespace UniSim;

namespace vg {

PUBLISH(CropGrowth)

/*! \class CropGrowth
 * \brief Crop growth rate and energy budget
 *
 * Inputs
 * ------
 * - _respRoot_ is the root basic respiration at 25 <SUP>o</SUP>C [g CH<SUB>2</SUB>O/g/d]
 * - _respStem_ is the stem basic respiration at 25 <SUP>o</SUP>C [g CH<SUB>2</SUB>O/g/d]
 * - _respLeaf_ is the leaf basic respiration at 25 <SUP>o</SUP>C [g CH<SUB>2</SUB>O/g/d]
 * - _respFruit_ is the fruit basic respiration at 25 <SUP>o</SUP>C [g CH<SUB>2</SUB>O/g/d]
 * - _massRoot_ is root dry mass [g/m<SUP>2</SUP>]
 * - _massStem_ is stem dry mass [g/m<SUP>2</SUP>]
 * - _massLeaf_ is leaf dry mass [g/m<SUP>2</SUP>]
 * - _massFruit_ is fruit dry mass [g/m<SUP>2</SUP>]
 * - _timeStep_ is the integration time step [s]
 * - _Tcrop_ is crop temperature [<SUP>o</SUP>C]
 * - _Pgc_ is crop assimilation rate [g CO<SUB>2</SUB>/m<SUP>2</SUP>/h]
 *
 * Output
 * ------
 * - _maintenanceRespiration_ is the mass lost to maintenance in this time step [g/m<SUP>2</SUP>]
 * - _grossGrowthRate_ is the mass allocated to growth and growth respiration in this time step [g/m<SUP>2</SUP>]
 *
 * Default dependencies
 * ------------
 * - a _mass_ sibling model with a four ports:
 * + _root_ [g/m<SUP>2</SUP>]
 * + _stem_ [g/m<SUP>2</SUP>]
 * + _leaf_ [g/m<SUP>2</SUP>]
 * + _fruit_ [g/m<SUP>2</SUP>]
 * - a _temperature_ sibling model with a _value_ port [<SUP>o</SUP>C]
 * - an _Pgc_ child model with a _value_ port [g CO<SUB>2</SUB> /m<SUP>2</SUP>/h]
 * - an _calendar_ model with an _timeStepSecs_ port [s]
 */

CropGrowth::CropGrowth(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, respRoot, 0.03);
    Input(double, respStem, 0.03);
    Input(double, respLeaf, 0.015);
    Input(double, respFruit, 0.01);
    InputRef(double, massRoot, "../mass[root]");
    InputRef(double, massStem, "../mass[stem]");
    InputRef(double, massLeaf, "../mass[leaf]");
    InputRef(double, massFruit, "../mass[fruit]");
    InputRef(double, Tcrop, "../temperature[value]");
    InputRef(double, Pgc, "./Pgc[value]");
    InputRef(double, timeStep, "calendar[timeStepSecs]");

    Output(double, maintenanceRespiration);
    Output(double, grossGrowthRate);
}

void CropGrowth::reset() {
    maintenanceRespiration = grossGrowthRate = 0.;
}

void CropGrowth::update() {
    const double Q10 = 2.;
    double dt = timeStep/3600; // [h]
    maintenanceRespiration = maintenanceRespirationRate()*dt*pow(Q10, 0.1*(Tcrop-25.));
    grossGrowthRate = max(Pgc*dt*30./44. - maintenanceRespiration, 0.);
}

double CropGrowth::maintenanceRespirationRate() {
    // At 25 oC [g CH2O/d]
    double rate = respRoot*massRoot +
                  respStem*massStem +
                  respLeaf*massLeaf +
                  respFruit*massFruit;
    // At 25 oC [g CH2O/h]
    return rate/24.;

}

} //namespace

