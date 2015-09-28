/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "floor_radiation_absorbed.h"
#include "publish.h"

using std::max;
using namespace UniSim;

namespace vg {

PUBLISH(FloorRadiationAbsorbed)


/*! \class FloorRadiationAbsorbed
 * \brief
 *
 * Inputs
 * ------

 * Output
 * ------
 *
 */

FloorRadiationAbsorbed::FloorRadiationAbsorbed(Identifier name, QObject *parent)
    : Model(name, parent)
{
    InputRef(double, indoorsLight, "indoors/light[total]");
    InputRef(double, growthLightLight, "actuators/growthlights[shortWaveEmission]");
    InputRef(double, lightAbsorbedByCrop, "crop/lightAbsorbed[value]");
    InputRef(double, growthLightLwAbsorbedByCrop, "crop/growthLightLwAbsorbed[value]");
    Output(double, value);
}

void FloorRadiationAbsorbed::reset() {
    value = 0.;
}

void FloorRadiationAbsorbed::update() {
    value = max(indoorsLight + growthLightLight - lightAbsorbedByCrop, 0.) +
            max(growthLightLw - growthLightLwAbsorbedByCrop, 0.);
}

} //namespace

