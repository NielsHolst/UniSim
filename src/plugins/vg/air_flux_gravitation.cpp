/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "air_flux_gravitation.h"
#include "general.h"
#include "publish.h"

using namespace UniSim;
using std::min;

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
 * - _value_ is the air exchange rate [m<SUP>3</SUP>/h]
 */

AirFluxGravitation::AirFluxGravitation(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, state, "construction/shelters[screensMaxState]");
    InputRef(double, topTemperature, "outdoors[temperature]");
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, indoorsVolume, "geometry[indoorsVolume]");
    InputRef(double, groundArea, "geometry[groundArea]");
    Output(double, value);
}

void AirFluxGravitation::reset() {
    value = 0.;
}

void AirFluxGravitation::update() {
    double gapArea = (1. - state)*groundArea,
           topRho = rhoAir(topTemperature),
           bottomRho = rhoAir(indoorsTemperature),
           flowRate = (indoorsTemperature > topTemperature) ?  // m3/h
                      0.06*g*pow(gapArea, 1.5)*sqrt((topRho - bottomRho)/topRho)*state*2 : 0.;

    value = flowRate/indoorsVolume;
}

} //namespace

