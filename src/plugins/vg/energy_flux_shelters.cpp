/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "energy_flux_shelters.h"
#include "general.h"
#include "publish.h"

using std::max;
using namespace UniSim;

namespace vg {
	
PUBLISH(EnergyFluxShelters)

/*! \class EnergyFluxShelters
 * \brief Pooled energy fluxes and temperature of greenhouse shelters
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
EnergyFluxShelters::EnergyFluxShelters(Identifier name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    InputRef(double, U, "construction/shelters[U]");
    InputRef(double, incomingLwAbsorptivity, "construction/shelters[incomingLwAbsorptivity]");
    InputRef(double, outgoingLwAbsorptivity, "construction/shelters[outgoingLwAbsorptivity]");

    InputRef(double, lightAbsorbedCover, "construction/shelters[lightAbsorbedCover]");
    InputRef(double, lightAbsorbedScreens, "construction/shelters[lightAbsorbedScreens]");

    InputRef(double, heatCapacityCover, "construction/shelters[heatCapacityCoversPerGround]");
    InputRef(double, heatCapacityScreens, "construction/shelters[heatCapacityScreensPerGround]");

    InputRef(double, timeStep, "calendar[timeStepSecs]");
    InputRef(double, averageHeight,"geometry[indoorsAverageHeight]");
    InputRef(double, coverPerGroundArea,"geometry[coverPerGroundArea]");

    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, outdoorsTemperature, "outdoors[temperature]");
    InputRef(double, skyTemperature, "outdoors[skyTemperature]");
    InputRef(double, radiationFluxCropTop, "crop/layers/top/radiationAbsorbed[shelterLoss]");
    InputRef(double, radiationFluxCropMiddle, "crop/layers/middle/radiationAbsorbed[shelterLoss]");
    InputRef(double, radiationFluxCropBottom, "crop/layers/bottom/radiationAbsorbed[shelterLoss]");

    Output(double, heatFluxOutsideToCover);
    Output(double, heatFluxInsideToCover);
    Output(double, radiationFluxSkyToCover);
    Output(double, radiationFluxSunToCover);
    Output(double, radiationFluxSunToScreens);
    Output(double, coverTemperature);
    Output(double, screensTemperature);
}

void EnergyFluxShelters::reset() {
    EnergyFluxBase::reset();
    heatFluxOutsideToCover =
    heatFluxInsideToCover =
    radiationFluxSkyToCover =
    radiationFluxSunToCover =
    radiationFluxSunToScreens = 0.;
    coverTemperature = screensTemperature = 12.;
}

void EnergyFluxShelters::update() {
    const int maxTimeStep = 20;     // Use time steps no larger than this [s]
    double Cair = averageHeight*RhoAir*CpAir;
    int n = max(int(timeStep/maxTimeStep), 1);
    double dt = timeStep/n;
    value = 0.;
    double indoorsTemperature2 = indoorsTemperature;
    // All fluxes are signed relative to the shelter:
    //  a positive flux means shelter is gaining energy
    //  a negative flux means shelter is loosing energy
    for (int i=0; i < n; ++i) {
        screensTemperature = (coverTemperature + indoorsTemperature2)/2;
        // W/m2 ground = W/m2 cover * m2 cover / m2 ground
        heatFluxOutsideToCover = U*(outdoorsTemperature - coverTemperature)*coverPerGroundArea;;
        heatFluxInsideToCover = U*(indoorsTemperature2 - coverTemperature)*coverPerGroundArea;;
        radiationFluxSkyToCover = incomingLwAbsorptivity*Sigma*(p4K(skyTemperature) - p4K(coverTemperature))*coverPerGroundArea;


        radiationFluxSunToCover = lightAbsorbedCover;
        radiationFluxSunToScreens = lightAbsorbedScreens;

        double radiationFluxCropToShelter =
            radiationFluxCropTop + radiationFluxCropMiddle + radiationFluxCropBottom;
        double radiationFluxToShelter =
                heatFluxOutsideToCover +
                heatFluxInsideToCover +
                radiationFluxSkyToCover +
                radiationFluxSunToCover +
                radiationFluxSunToScreens +
                radiationFluxCropToShelter;
        double radiationFluxToInside =
                -heatFluxInsideToCover;

        value += radiationFluxToInside;
        coverTemperature += radiationFluxToShelter*dt/(heatCapacityCover+heatCapacityScreens);
        indoorsTemperature2 += radiationFluxToInside*dt/Cair;
    }
    value /= n;
}

} //namespace

