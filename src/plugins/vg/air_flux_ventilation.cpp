/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "air_flux_ventilation.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(AirFluxVentilation)

/*! \class AirFluxVentilation
 * \brief The proportional air exchange rate with the outside
 *
 * Inputs
 * ------
 * - _infiltration_ is the infiltration air exchange rate [h<SUP>-1</SUP>]
 * - _crack_ is the air exchange rate through the humidity-controlled ventilation crack [h<SUP>-1</SUP>]
 * - _propOfCrack_ is the proportion of the crack ventilation that is effective [0;1]
 *
 * Output
 * ------
 * - _value_ is the proportional air exchange rate with the outside [h<SUP>-1</SUP>]
 */

AirFluxVentilation::AirFluxVentilation(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, infiltration, 0.);
    Input(double, crack, 0.);
    Input(double, propOfCrack, 0.);
    Output(double, value);
}

void AirFluxVentilation::reset() {
    value = 0.;
}

void AirFluxVentilation::update() {
    value = infiltration + propOfCrack*crack;
}


} //namespace

