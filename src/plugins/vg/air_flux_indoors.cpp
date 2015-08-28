/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "air_flux_indoors.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(AirFluxIndoors)

/*! \class AirFluxIndoors
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

AirFluxIndoors::AirFluxIndoors(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, infiltration, "total/airFlux/infiltration[value]");
    InputRef(double, gravitation, "total/airFlux/gravitation[value]");
    InputRef(double, airTransmissivity, "shelters/roof[airTransmissivity]");
    Input(double, receiverVolume, 0.);
    Output(double, value);
}

void AirFluxIndoors::reset() {
    value = 0.;
}

void AirFluxIndoors::update() {
    value = airTransmissivity*infiltration + gravitation/receiverVolume;
}


} //namespace

