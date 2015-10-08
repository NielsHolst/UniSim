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
 * - _U_ is the overall heat transfer coefficient of the shelters [W/m<SUP>2</SUP>/K]
 * - _incomingLwAbsorptivity_ is the absorptivity for long-waved radiation coming from the outdoors direction [0;1]
 * - _outgoingLwAbsorptivity_ is the absorptivity for long-waved radiation going in the outdoors direction [0;1]
 * - _lightAbsorbedCover_ is the intensity of sunlight absorbed by the cover [W/m<SUP>2</SUP> ground]
 * - _lightAbsorbedScreens_ is the intensity of sunlight absorbed by the screens [W/m<SUP>2</SUP> ground]
 * - _heatCapacityCoversPerGround_ is the total heat capacity of all covers [J/kg/m<SUP>2</SUP> ground]
 * - _heatCapacityScreensPerGround_ is the total heat capacity of all screens [J/kg/m<SUP>2</SUP> ground]
 * - _timeStep_ is the integration time step [s]
 * - _height_ is the average height of the greenhouse [m]
 * - _coverPerGroundArea_ is the total cover area per ground area [m<SUP>2</SUP> cover/m<SUP>2</SUP> ground]
 * - _indoorsTemperature_ is the ambient temperature indoors [<SUP>o</SUP>C]
 * - _outdoorsTemperature_ is the ambient temperature outdoors [<SUP>o</SUP>C]
 * - _skyTemperature_ is the temperature of the sky [<SUP>o</SUP>C]
 * - _radiationFluxCropTop_ is the flux of long-waved radiation going from the top leaf layer to the shelter [W/m<SUP>2</SUP>]
 * - _radiationFluxCropMiddle_ is the flux of long-waved radiation going from the middle leaf layer to the shelter [W/m<SUP>2</SUP>]
 * - _radiationFluxCropBottom_ is the flux of long-waved radiation going from the bottom leaf layer to the shelter [W/m<SUP>2</SUP>]
 *
 * Outputs
 * ------
 * All fluxes below are per greenhouse ground area.
 * - _heatFluxOutsideToCover_ is the energy flux gained by the cover from the outside [W/m<SUP>2</SUP>]
 * - _heatFluxInsideToCover_ is the energy flux gained by the cover from the inside [W/m<SUP>2</SUP>]
 * - _radiationFluxSkyToCover_ is the energy flux gained by cover from the sky (negative) [W/m<SUP>2</SUP>]
 * - _radiationFluxSunToCover_ is the energy flux gained by cover from the sun (positive) [W/m<SUP>2</SUP>]
 * - _radiationFluxSunToScreens_ is the energy flux gained by the screens from the sun (positive) [W/m<SUP>2</SUP>]
 * - _coverTemperature_ is the cover temperature [<SUP>o</SUP>C]
 * - _screensTemperature_ is the screens temperature [<SUP>o</SUP>C]
 * - _value_ is the net energy flux from shelters to the greenhouse air  [W/m<SUP>2</SUP>]

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
    InputRef(double, height, "geometry[indoorsAverageHeight]");
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
    double Cair = height*RhoAir*CpAir;
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

