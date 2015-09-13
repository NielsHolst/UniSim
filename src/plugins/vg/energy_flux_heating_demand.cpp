/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "energy_flux_heating_demand.h"
#include "general.h"
#include "publish.h"

using namespace UniSim;
using std::max;

namespace vg {

PUBLISH(EnergyFluxHeatingDemand)

/*! \class EnergyFluxHeatingDemand
 * \brief Flux of energy need to warm up to heating setpoint
 * Inputs
 * ------
 * - _passiveTemperature_ is the indoors tempeature reached, it not actively regulated [<SUP>o</SUP>C]
 * - _heatingSetpoint_ is the heating setpoint [<SUP>oC</SUP>C]
 * - _averageHeight_ is the average height of the greenhouse [m]
 * - _timeStep_ is the integration time step [s]
 *
 * Output
 * ------
 * - _value_ is the energy flux needed to heat the greenhouse to the heating setpoint [W/m<SUP>2</SUP>]
 */

EnergyFluxHeatingDemand::EnergyFluxHeatingDemand(Identifier name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    InputRef(double, passiveEnergyFlux, "given/energyFlux[value]");
    InputRef(double, heatingSetpoint, "setpoints/temperature/heating[value]");
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, averageHeight, "geometry[indoorsAverageHeight]");
    InputRef(double, timeStep,"calendar[timeStepSecs]");
}

void EnergyFluxHeatingDemand::update() {
    double dT = max(heatingSetpoint - indoorsTemperature, 0.);
    value = max(dT*CpAir*RhoAir*averageHeight/timeStep - passiveEnergyFlux, 0.);
}

} //namespace

