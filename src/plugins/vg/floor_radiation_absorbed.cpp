/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "floor_radiation_absorbed.h"
#include "publish.h"

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
    InputRef(double, outdoorsRadiation, "outdoors[radiation]");
    InputRef(double, cropRadiationAbsorbed, "crop/radiationAbsorbed[value]");
    Output(double, value);
}

void FloorRadiationAbsorbed::reset() {
    value = 0.;
}

void FloorRadiationAbsorbed::update() {
    value = std::max(outdoorsRadiation - cropRadiationAbsorbed, 0.);
}

} //namespace

