/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "air_flux_infiltration.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(AirFluxInfiltration)

/*! \class AirFluxInfiltration
 * \brief Air infiltration through greenhouse leakages
 *
 * Inputs
 * ------
 * - _leakage_ is the infiltration at a windspeed of 4 m/s [h<SUP>-1</SUP>]
 * - _windspeed_ is the outdoors windspeed [m/s]
 * - _airTransmissivity_ is the air transmissivity of the greenhouse shelter [0;1]
 *
 * Output
 * ------
 * - _value_ is the relative rate of air exchanged [h<SUP>-1</SUP>]
 */

AirFluxInfiltration::AirFluxInfiltration(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, leakage, 0.5);
    InputRef(double, windspeed, "outdoors[windspeed]");
    InputRef(double, airTransmissivity, "construction/shelters[airTransmissivity]");
    Output(double, value);
}

void AirFluxInfiltration::reset() {
    value = 0.;
}

void AirFluxInfiltration::update() {
    value = leakage*airTransmissivity*windspeed/4;
}


} //namespace

