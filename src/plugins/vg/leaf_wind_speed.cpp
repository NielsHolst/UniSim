/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "leaf_wind_speed.h"
#include "publish.h"

using std::max;
using namespace UniSim;

namespace vg {

PUBLISH(LeafWindSpeed)

/*! \class LeafWindSpeed
 * \brief Boundary layer resistance against H<SUB>2</SUB>O and CO<SUB>2</SUB>
 *
 * Inputs
 * ------
 * - _k_ is the extinction coefficient for wind down throught the canopy [-]
 * - _indoorsWindSpeedMinimum_ is the assumed minimum limit to wind speed indoors [m/s]
 * - _indoorsWindSpeed_ is the wind speed indoors [m/s]
 * - _xGauss_ is the coefficient for Gaussian integration from the upper side of the canopy [0;1]
 * - _lai_ is the crop leaf area index [m<SUP>2</SUP>/m<SUP>2</SUP>]
 *
 * Outputs
 * ------
 * - _value_ is the wind speed at leaf level [m/s]
 */

LeafWindSpeed::LeafWindSpeed(Identifier name, QObject *parent)
    : Model(name, parent)
{
    Input(double, k, 0.);
    Input(double, indoorsWindSpeedMinimum, 0.025);
    InputRef(double, indoorsWindSpeed, "indoors/windSpeed[value]");
    InputRef(double, xGauss, "..[xGaussUpperside]");
    InputRef(double, lai, "crop/lai[value]");
    Output(double, value);
}

void LeafWindSpeed::reset() {
    value = indoorsWindSpeedMinimum;
}

void LeafWindSpeed::update() {
    value = max(indoorsWindSpeed, indoorsWindSpeedMinimum)*exp(-k*lai*xGauss);
}


} //namespace

