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
 * \brief Air infiltration is the rate of air entering the greenhouse uncontrolled, i.e. in addition to active ventilation
 *
 * Inputs
 * ------
 * - _leakage_ is infiltration through leakages [m<SUP>3</SUP> air/m<SUP>3</SUP> greenhouse/h]
 * - _greenhouseVolume_ is the volume of the greenhouse [m<SUP>3</SUP>]
 * - _greenhouseArea_ is the ground area occupied by the greenhouse [m<SUP>2</SUP>]
 * - _windspeed_ is the outdoors windspeed [m/s]
 * - _screensAirTransmission_ is the proportion of air led through the screens [0;1]
 *
 * Output
 * ------
 * - _rate_ is the infiltration rate per greenhouse area [m<SUP>3</SUP> air/h/m<SUP>2</SUP>]
 *
 * Default dependencies
 * ------------
 * - a _construction/geometry_ model with two ports:
 *   + _volumeTotal_ [m<SUP>3</SUP>]
 *   + _groundArea_ [m<SUP>2</SUP>]
 * - an _outdoors_ model with a _windspeed_ port [m/s]
 * - an _indoors/screens/transmission_ model witn an _air_ port [0;1]
 */

AirInfiltration::AirInfiltration(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, leakage, 0.5);
    InputRef(double, greenhouseVolume, "construction/geometry[volumeTotal]");
    InputRef(double, greenhouseArea, "construction/geometry[groundArea]");
    InputRef(double, windspeed, "outdoors[windspeed]");
    InputRef(double, screensAirTransmission, "indoors/screens/transmission[air]");
    Output(double, rate);
}

void AirInfiltration::reset() {
    rate = 0.;
}

void AirInfiltration::update() {
    rate = greenhouseVolume*leakage*screensAirTransmission*windspeed/4/greenhouseArea;
}


} //namespace

