/* Copyright (C) 2013 by Oliver Koerner, AgroTeh [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "energy_flux_floor.h"
#include "general.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(EnergyFluxFloor)

/*! \class EnergyFluxFloor
 * \brief Greenhouse floor temperature and energy flux to greenhouse
 * Inputs
 * ------
 * - _U_ is the U-value from greenhouse air to floor [W/m<SUP>2</SUP>]/K]
 * - _heatCapacity_ is the heat capacity of the floor [J/m<SUP>2</SUP>]/K]
 * - _radiationAbsorbed_ is the sunlight not absorbed by the crop [W/m<SUP>2</SUP>]
 * - _indoorsTemperature_ is the ambient temperature indoors [<SUP>oC</SUP>C]
 * - _soilTemperature_ is the outside soil temperature  [<SUP>oC</SUP>C]
 * - _averageHeight_ is the average height of the greenhouse (volume divided by ground area) [m]
 * - _timeStep_ is the integration time step [s]
 *
 * Output
 * ------
 * - _value_ is the energy flux per greenhouse area from the floor [W/m<SUP>2</SUP>]
 * - _temperature_ is the floor temperature [<SUP>oC</SUP>C]
 */

EnergyFluxFloor::EnergyFluxFloor(Identifier name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    Input(double, U, 7.5);
    Input(double, heatCapacity, 42000.);
    InputRef(double, radiationAbsorbed, "./radiationAbsorbed[value]");
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, soilTemperature, "outdoors[soilTemperature]");
    InputRef(double, averageHeight,"geometry[indoorsAverageHeight]");
    InputRef(double, timeStep, "calendar[timeStepSecs]");
    Output(double, temperature);
}

void EnergyFluxFloor::reset() {
    EnergyFluxBase::reset();
    temperature = 18.;
}

void EnergyFluxFloor::update() {
    const int maxTimeStep = 20;     // Use time steps no larger than this [s]
    int n = int(timeStep/maxTimeStep) + 1;
    double dt = timeStep/n,
           Tin = indoorsTemperature,
           Cair = averageHeight*RhoAir*CpAir;               // J/m2/K = m * kg/m3 * J/kg/K

    value = 0.;
    temperature += radiationAbsorbed*dt/heatCapacity;
    for (int i=0; i<n; ++i) {
        double fluxFloorToSoil = U*(temperature-soilTemperature),
               fluxFloorToAir = U*(temperature-Tin);
        temperature -= (fluxFloorToSoil + fluxFloorToAir)*dt/heatCapacity;
        Tin += fluxFloorToAir*dt/Cair;
        value += fluxFloorToAir;
    }
    value /= n;
}

} //namespace

