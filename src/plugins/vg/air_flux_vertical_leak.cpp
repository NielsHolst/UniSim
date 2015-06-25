/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "air_flux_vertical_leak.h"
#include "general.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(AirFluxVerticalLeak)

/*! \class AirFluxVerticalLeak
 * \brief The obligatory air flux is leakage plus humidity-controlled ventilation
 *
 * Inputs
 * ------
 * - _leakage_ is infiltration through leakages [m<SUP>3</SUP> air/m<SUP>3</SUP> greenhouse/h]
 * - _averageHeight_ is the average height of the greenhouse [m<SUP>3</SUP>/m<SUP>2</SUP>]
 * - _windspeed_ is the outdoors windspeed [m/s]
 * - _screensAirTransmission_ is the proportion of air led through the screens [0;1]
 *
 * Output
 * ------
 * - _value_ is the proportional air exchange [h<SUP>-1</SUP>]
 */

AirFluxVerticalLeak::AirFluxVerticalLeak(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, separation, 0.);
    Input(double, screensAirTransmission, 1.);
    Input(double, indoorsWindSpeed, 0.05);
    InputRef(double, greenhouseArea, "construction/geometry[groundArea]");
    Output(double, value);
}

void AirFluxVerticalLeak::reset() {
    value = 0.;
}

void AirFluxVerticalLeak::update() {
    value = screensAirTransmission*indoorsWindSpeed*greenhouseArea*3600.; // h-1 = m/s * m2 * s/h
}


} //namespace

