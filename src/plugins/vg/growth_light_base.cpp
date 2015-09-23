/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "growth_light_base.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(GrowthLightBase)

/*! \class GrowthLightBase
 * \brief Base class for growth light
 *
 * Output
 * ------
 * - _heatEmission_ is the convective heat emission [W/m<SUP>2</SUP>]
 * - _longWaveEmission_ is the long wave emission (thermal radiation, Bn) [W/m<SUP>2</SUP>]
 * - _shortWaveEmission_ is the short wave emission (visible light, Rn) [W/m<SUP>2</SUP>]
 * - _totalEmission_ is the total of _heatEmission_+_longWaveEmission_+_shortWaveEmission_  [W/m<SUP>2</SUP>]
 * - _parEmission_ is the PAR emision [W/m<SUP>2</SUP>]
 * - _energyFlux_ is the energy used for growth lights [W/m<SUP>2</SUP>]
 * - _currentlyOn_ tells whether light is on [true/false]
 */

GrowthLightBase::GrowthLightBase(Identifier name, QObject *parent)
    : Model(name, parent)
{
    Output(double, heatEmission);
    Output(double, longWaveEmission);
    Output(double, shortWaveEmission);
    Output(double, totalEmission);
    Output(double, parEmission);
    Output(double, energyFlux);
    Output(bool, currentlyOn);
}

void GrowthLightBase::noLight() {
    heatEmission =
    longWaveEmission =
    shortWaveEmission =
    totalEmission =
    parEmission =
    energyFlux = 0.;
    currentlyOn = false;
}

} //namespace

