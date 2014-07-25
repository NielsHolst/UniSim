/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "humidity_setpoints.h"
#include "publish.h"

using namespace UniSim;

namespace vg {
	
PUBLISH(HumiditySetpoints)

/*! \class HumiditySetpoints
 * \brief Computes humidity setpoints in terms of relative and absolute humidity
 *
 * Inputs
 * ------
 * - _maxRhDay_ is the r.h. maximum setpoint in the day [0-100]
 * - _maxRhNight_ is the r.h. maximum setpoint at night [0-100]
 * - _minDeltaX_ is the minimum delta-x setpoint [g/m<SUP>3</SUP>]
 * - _isDay_tells if it is day or night [true,false]
 * - _radiation_ is the sunlight irradiation [W/m<SUP>2</SUP>]
 *
 * Outputs
 * ------
 * - _maxRh_ is the r.h. maximum setpoint [0-100]
 * - _minDeltaX_ is the delta-x minimum setpoint [g/m<SUP>3</SUP>]

 * Default dependencies
 * ------------
 * - an _outdoors_ model with a _radiation_ port [W/m<SUP>2</SUP>]
 */

HumiditySetpoints::HumiditySetpoints(Identifier name, QObject *parent)
    : Model(name, parent)
{
    Input(double, maxRhDay, 80.);
    Input(double, maxRhNight, 90.);
    Input(double, minDeltaX, 1.);
    InputRef(bool, isDay, "./day[isDay]");
    InputRef(double, radiation, "outdoors[radiation]");
    Output(double, maxRh);
    Output(double, minDeltaX);
}

void HumiditySetpoints::reset() {
    maxRh = maxRhNight;
}

void HumiditySetpoints::update() {
    // Setpoints
    maxRh = isDay ? maxRhNight : maxRhDay;
    minDeltaX = minDeltaXBasis;
}

} //namespace

