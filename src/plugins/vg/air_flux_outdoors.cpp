/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "air_flux_outdoors.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(AirFluxOutdoors)

/*! \class AirFluxOutdoors
 * \brief The proportional air exchange rate with the outside
 *
 * Inputs
 * ------
 * - _infiltration_ is the infiltration air exchange rate [h<SUP>-1</SUP>]
 * - _crackVentilation_ is the air exchange rate through the humidity-controlled ventilation crack [h<SUP>-1</SUP>]
 * - _proportion_ is the proportion of the crack ventilation that is effective in this compartment [0;1]
 *
 * Output
 * ------
 * - _value_ is the proportional air exchange rate with the outside [h<SUP>-1</SUP>]
 */

AirFluxOutdoors::AirFluxOutdoors(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, infiltration, 0.);
    Input(double, ventilation, 0.);
    Input(double, volumeProportion, 0.);
    Input(double, gap, 1.);
    Output(double, value);
}

void AirFluxOutdoors::reset() {
    value = 0.;
}

void AirFluxOutdoors::update() {
    value = volumeProportion*infiltration + gap*ventilation;
}


} //namespace

