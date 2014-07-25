/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "boundary_layer_resistance.h"
#include "publish.h"

using namespace std;
using namespace UniSim;

namespace vg {

PUBLISH(BoundaryLayerResistance)

/*! \class BoundaryLayerResistance
 * \brief Boundary layer resistance against H<SUB>2</SUB>O and CO<SUB>2</SUB>
 *
 * Inputs
 * ------
 * - _windSpeed_ is outdoors wind speed [m/s]
 * - _ventilation_ is the rate of air exchange through ventilation [m<SUP>3</SUP>]/m<SUP>2</SUP>/h]
 *
 * Outputs
 * ------
 * - _rbH2O_ is the boundary layer resistance against water vapour [s/m]
 * - _rbCO2_ is the boundary layer resistance against CO<SUB>2</SUB> [s/m]
 *
 * Default dependencies
 * ------------
 * - an _outdoors_ model with a _windspeed_ port [m/s]
 * - an _indoors/ventilation_ model with a _rate_ port [m<SUP>3</SUP>]/m<SUP>2</SUP>/h]
 */


BoundaryLayerResistance::BoundaryLayerResistance(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, windSpeed, "outdoors[windspeed]");
    InputRef(double, ventilation, "indoors/ventilation[rate]");
    Output(double, rbH2O);
    Output(double, rbCO2);
}

void BoundaryLayerResistance::reset() {
    updateValue(0.);
}

void BoundaryLayerResistance::update() {
    updateValue(0.1*windSpeed/4.*ventilation/3600.);
}

void BoundaryLayerResistance::updateValue(double windSpeed) {
    // Characteristic leaf dimension
    const double L = 25./1000.;
    // (Stanghellini GCC p 146 modified)
    rbH2O = 200.*sqrt( L/max(0.05,windSpeed) );
    rbCO2 = rbH2O*1.37;
}

} //namespace

