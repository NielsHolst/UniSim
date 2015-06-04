/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "air_infiltration.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(AirInfiltration)

/*! \class AirInfiltration
 * \brief Air infiltration is the rate of air entering the greenhouse uncontrolled
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

AirInfiltration::AirInfiltration(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, leakage, 0.5);
    InputRef(double, windspeed, "outdoors[windspeed]");
    InputRef(double, screensAirTransmission, "greenhouseShelter[airTransmission]");
    Output(double, value);
}

void AirInfiltration::reset() {
    value = 0.;
}

void AirInfiltration::update() {
    value = leakage*screensAirTransmission*windspeed/4;
}


} //namespace

