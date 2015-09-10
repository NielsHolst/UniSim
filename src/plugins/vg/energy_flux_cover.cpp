/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "energy_flux_cover.h"
#include "general.h"
#include "publish.h"

using namespace UniSim;

namespace vg {
	
PUBLISH(EnergyFluxCover)

/*! \class EnergyFluxCover
 * \brief Energy fluxes and temperature of greenhouse cover
 *
 * Inputs
 * ------
 * - _U_ is the heat transfer coefficient of the material  [W/m<SUP>2</SUP>/K]
 * - _emissivity_ is the effectiveness in emitting energy as thermal radiation [0;1].
 * - _absorptivity_ is the proportion of light absorbed [0;1]
 * - _heatCapacity_ is cover heat capacity [J/m<SUP>2</SUP>]/K]
 * - _timeStep_ is the integration time step [s]
 * - _indoorsTemperature_ is the ambient temperature indoors [<SUP>o</SUP>C]
 * - _outdoorsTemperature_ is the ambient temperature outdoors [<SUP>o</SUP>C]
 * - _skyTemperature_ is the temperature of the sky [<SUP>o</SUP>C]
 * - _screenState_ is the max. state of the screens below this cover [0;1], where 0=Open and 1=Drawn
 * - _cropTemperature_ is the temperature of the top layer of the crop [<SUP>o</SUP>C]
 * - _sunlight_ is the outdoors radiation [W/m<SUP>2</SUP]
 * - _condensation_ is the condensation rate on the cover [kg/m<SUP>2</SUP>/s]
 *
 * Outputs
 * ------
 * All fluxes below are per greenhouse groubd area.
 * - _heatFluxOutside_ is the convective energy flux to the cover from the outside, usually negative [W/m<SUP>2</SUP>]
 * - _heatFluxInside_ is the convective energy flux to the cover from the inside, usually positive [W/m<SUP>2</SUP>]
 * - _radiationFluxSky_ is the radiative energy flux to the cover from the sky, always negative [W/m<SUP>2</SUP>]
 * - _radiationFluxSun_ is the radiative energy flux to the cover from the sun, positive in the day [W/m<SUP>2</SUP>]
 * - _radiationFluxScreen_ is the radiative energy flux to the cover from the screen [W/m<SUP>2</SUP>]
 * -
 * -
 * - _value_ is the net energy flux from cover to greenhouse air  [W/m<SUP>2</SUP>]

 */
EnergyFluxCover::EnergyFluxCover(Identifier name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    InputRef(double, U, "..[U]");
    InputRef(double, emissivity, "..[emissivity]");
    InputRef(double, absorptivity, "..[absorptivity]");
    InputRef(double, heatCapacity, "..[heatCapacity]");
    InputRef(double, timeStep, "calendar[timeStepSecs]");
    InputRef(double, averageHeight,"geometry[indoorsAverageHeight]");
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, outdoorsTemperature, "outdoors[temperature]");
    InputRef(double, skyTemperature, "outdoors[skyTemperature]");
    InputRef(double, screenState, "../../screens[maxState]");
    InputRef(double, cropTemperature, "crop/layers/top/temperature[value]");
    InputRef(double, sunlight, "outdoors[radiation]");
    InputRef(double, condensationRate, "../condensation[vapourFlux]");
    Output(double, heatFluxOutside);
    Output(double, heatFluxInside);
    Output(double, radiationFluxSky);
    Output(double, radiationFluxSun);
    Output(double, radiationFluxScreen);
    Output(double, radiationFluxCrop);
    Output(double, temperature);
}

void EnergyFluxCover::reset() {
    EnergyFluxBase::reset();
    temperature = 12.;
    heatFluxOutside =
    heatFluxInside =
    radiationFluxSky =
    radiationFluxSun =
    radiationFluxScreen =
    radiationFluxCrop = 0.;
}

void EnergyFluxCover::update() {
    const int maxTimeStep = 20;     // Use time steps no larger than this [s]
    double Cair = averageHeight*RhoAir*CpAir;
    int n = int(timeStep/maxTimeStep) + 1;
    double dt = timeStep/n;
    value = 0.;
    double indoorsTemperature2 = indoorsTemperature;
    // All fluxes are signed relative to the cover:
    //  a positive flux means cover is gaining energy
    //  a negative flux means cover is loosing energy
    for (int i=0; i < n; ++i) {
        double screenTemperature = (temperature + indoorsTemperature2)/2;
        heatFluxOutside = U*(outdoorsTemperature - temperature);                       // W/m2 = W/m2/K * K =
        heatFluxInside = U*(indoorsTemperature2 - temperature);
        radiationFluxSky = emissivity*Sigma*(p4K(skyTemperature) - p4K(temperature));    // W/m2 = 1 * W/m2/K4 * K4
        radiationFluxSun = absorptivity*sunlight;
        radiationFluxScreen = emissivity*Sigma*(p4K(screenTemperature) - p4K(temperature))*(1 - screenState);
        radiationFluxCrop = emissivity*Sigma*(p4K(cropTemperature) - p4K(temperature))*screenState;
        double totalGlassFlux =
           heatFluxOutside +
           heatFluxInside +
           radiationFluxSky +
           radiationFluxSun +
           radiationFluxScreen +
           radiationFluxCrop +
           condensationRate*LHe;    // W/m2 = kg/m2/s * J/kg
        double totalInsideFlux =
            heatFluxInside +
            radiationFluxScreen +
            radiationFluxCrop +
            condensationRate*LHe;
        value -= totalInsideFlux;
        heatCapacity = 8000.;
        temperature += totalGlassFlux*dt/heatCapacity;    // K = W/m2*s / (J/m2/K)
        indoorsTemperature2 -= totalInsideFlux*dt/Cair;
    }
    value /= n;
}


} //namespace

