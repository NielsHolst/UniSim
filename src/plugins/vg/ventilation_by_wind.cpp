/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "ventilation_by_wind.h"
#include "general.h"
#include "publish.h"

using std::min;
using namespace UniSim;

namespace vg {

PUBLISH(VentilationByWind)

/*! \class VentilationByWind
 * \brief Maximum ventilation possible due to outdoors wind
 * Inputs
 * ------
 * - _baseRate_ is the linear increase in ventilation rate with increasing wind speed [h<SUP>-1</SUP> s/m]
 * - _windspeed_ is outdoors wind speed [m/s]
 * - _ventsPoportionalEffectiveArea_ is the total effective ventilation area per ground area [m<SUP>2</SUP> ventilation/m<SUP>2</SUP> ground]
 *
 * Output
 * ------
 * - _value_ is the maximum ventilation possible due to outdoors wind [h<SUP>-1</SUP>]
 */

VentilationByWind::VentilationByWind(Identifier name, QObject *parent)
    : Model(name, parent)
{
    Input(double, baseRate, 30.);
    InputRef(double, windspeed, "outdoors[windspeed]");
    InputRef(double, ventsProportionalEffectiveArea, "construction/vents[proportionalEffectiveArea]");
    Output(double, value);
}

void VentilationByWind::reset() {
    value = baseRate;
}

void VentilationByWind::update() {
    value = baseRate*windspeed*ventsProportionalEffectiveArea;
}

} //namespace

