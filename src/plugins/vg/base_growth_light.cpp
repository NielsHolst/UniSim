/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "base_growth_light.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(BaseGrowthLight)

/*! \class BaseGrowthLight
 * \brief Base class forgrowth light
 *
 * Output
 * ------
 * - _heatEmission_ is the convective heat emission [W/m<SUP>2</SUP>]
 * - _longWaveEmission_ is the long wave emission (thermal radiation, Bn) [W/m<SUP>2</SUP>]
 * - _shortWaveEmission_ is the short wave emission (visible light, Rn) [W/m<SUP>2</SUP>]
 * - _parEmission_ is the PAR emision [W/m<SUP>2</SUP>]
 * - _energyUse_ is the energy used [W/m<SUP>2</SUP>]
 *
 */

BaseGrowthLight::BaseGrowthLight(Identifier name, QObject *parent)
    : Model(name, parent)
{
    Output(double, heatEmission);
    Output(double, longWaveEmission);
    Output(double, shortWaveEmission);
    Output(double, parEmission);
    Output(double, energyUse);
    Output(bool, currentlyOn);
}

void BaseGrowthLight::noLight() {
    heatEmission =
    longWaveEmission =
    shortWaveEmission =
    parEmission =
    energyUse = 0.;
    currentlyOn = false;
}

} //namespace

