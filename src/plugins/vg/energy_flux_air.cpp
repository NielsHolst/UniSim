/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include "general.h"
#include "publish.h"
#include "energy_flux_air.h"

using namespace UniSim;

namespace vg {

PUBLISH(EnergyFluxAir)

/*! \class EnergyFluxAir
 * \brief Flux of energy carried by air from outdoors to indoors
 *
 * Inputs
 * ------
 * - _airFlux_ is the rate of air exchange between outdoors to indoors [h<SUP>-1</SUP>]
 * - _indoorsTemperature_ is the air temperature indoors [<SUP>oC</SUP>C]
 * - _outdoorsTemperature_ is the air temperature outdoors [<SUP>oC</SUP>C]
 * - _height_ is the average height of the greenhouse
 * - _timeStep_ is the integration time step [s]
*/

EnergyFluxAir::EnergyFluxAir(Identifier name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    Input(double, airFlux, 0.);
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, outdoorsTemperature, "outdoors[temperature]");
    InputRef(double, height,"geometry[indoorsAverageHeight]");
    InputRef(double, timeStep, "calendar[timeStepSecs]");
}

void EnergyFluxAir::update() {
    double Cair = height*CpAirVol,   // J/K/m2 = m * J/K/m3
           dT = outdoorsTemperature - indoorsTemperature;
    value = Cair*dT*airFlux/3600;    // W/m2 = J/K/m2 / h * h/s
}

} //namespace

