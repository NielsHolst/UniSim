/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "air_flux_given.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(AirFluxGiven)

/*! \class AirFluxGiven
 * \brief  The air flux given, irrespective of temperature-regulated ventilation

 *
 * Inputs
 * ------
 * - _infiltration_ is the infiltration air exchange rate [h<SUP>-1</SUP>]
 * - _crackVentilation_ is the air flux through the humidity-controlled ventilation crack [h<SUP>-1</SUP>]
 *
 * Output
 * ------
 * - _value_ is the relative rate of air exchanged [h<SUP>-1</SUP>]
 */

AirFluxGiven::AirFluxGiven(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, infiltration, "./infiltration[value]");
    InputRef(double, crackVentilation, "./crackVentilation[value]");
    InputRef(double, gravitation, "./gravitation[value]");
    InputRef(double, transmissivity, "construction/shelters[airTransmissivity]");
    Output(double, value);
}

void AirFluxGiven::reset() {
    value = 0.;
}

void AirFluxGiven::update() {
    value = infiltration + transmissivity*crackVentilation + gravitation;
}


} //namespace

