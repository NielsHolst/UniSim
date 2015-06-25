/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "air_flux_gravitation.h"
#include "general.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(AirFluxGravitation)

/*! \class AirFluxGravitation
 * \brief Vertical air movement by gravitation through screen gap
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

AirFluxGravitation::AirFluxGravitation(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, state, "horizontal/screens[maxState]");
    Input(double, topTemperature, 15.); // TEST TEST TEST
    InputRef(double, bottomTemperature, "indoors/temperature[value]");
    InputRef(double, greenhouseArea, "construction/geometry[groundArea]");
    InputRef(double, volume, "construction/geometry[volumeBelowRoof]");
    Output(double, value);
}

void AirFluxGravitation::reset() {
    value = 0.;
}

void AirFluxGravitation::update() {
    double gapArea = (1. - state)*greenhouseArea,
           topRho = rhoAir(topTemperature),
           bottomRho = rhoAir(bottomTemperature);
    value = (bottomTemperature > topTemperature) ?
            0.06*g*pow(gapArea, 1.5)*sqrt((topRho - bottomRho)/topRho)/volume : 0.;
}


} //namespace

