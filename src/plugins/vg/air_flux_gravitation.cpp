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
    InputRef(double, state, "horizontalScreens[maxState]");
    InputRef(double, topTemperature, "indoors/top/temperature[value]");
    InputRef(double, bottomTemperature, "indoors/temperature[value]");
    InputRef(double, topVolume, "construction/geometry[volumeTop]");
    InputRef(double, bottomVolume, "construction/geometry[volumeIndoors]");
    InputRef(double, greenhouseArea, "construction/geometry[groundArea]");
    InputRef(double, timeStep, "calendar[timeStepSecs]");
    Output(double, value);
}

void AirFluxGravitation::reset() {
    value = 0.;
}

void AirFluxGravitation::update() {
    double gapArea = (1. - state)*greenhouseArea,
           topRho = rhoAir(topTemperature),
           bottomRho = rhoAir(bottomTemperature);
    // m3/h
    double instantaneousRate =
           (bottomTemperature > topTemperature) ?
           0.06*g*pow(gapArea, 1.5)*sqrt((topRho - bottomRho)/topRho) : 0.;
    // m3
    double finiteRate = instantaneousRate*timeStep/3600;
    // m3/h
    value = maxFiniteRate(finiteRate)/timeStep*3600;
}

double AirFluxGravitation::maxFiniteRate(double finiteRate1) const {
    const double volumeStep = 5;
    double volume = 0,
           Ttop = topTemperature,
           Tbottom = bottomTemperature;
    while (volume < finiteRate1 && Tbottom > Ttop) {
        double dEtop = (Tbottom - Ttop)*CpAir*rhoAir(Tbottom)*volumeStep,
               dTtop = dEtop/CpAir/rhoAir(Ttop)/topVolume,
               dEbottom = (Ttop - Tbottom)*CpAir*rhoAir(Ttop)*volumeStep,
               dTbottom = dEbottom/CpAir/rhoAir(Tbottom)/bottomVolume;
        Ttop += dTtop;
        Tbottom += dTbottom;
        volume += volumeStep;
    }
    return min(finiteRate1, volume);
}


} //namespace

