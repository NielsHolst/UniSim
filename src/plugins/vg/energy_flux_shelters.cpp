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
    InputRef(double, incomingIrAbsorptivity, "construction/shelters[incomingIrAbsorptivity]");
//    InputRef(double, incomingIrAbsorptivityScreens, "../screens[incomingIrAbsorptivityScreens]");
    InputRef(double, outgoingIrAbsorptivity, "construction/shelters[outgoingIrAbsorptivity]");
//    InputRef(double, outgoingIrAbsorptivityCover, "../cover[outgoingIrAbsorptivity]");
//    InputRef(double, outgoingIrAbsorptivityScreens, "../screens[outgoingIrAbsorptivity]");

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
//    InputRef(double, maxState, "../../screens[maxState]");
    InputRef(double, cropTemperatureTop, "crop/layers/top/temperature[value]");
    InputRef(double, cropTemperatureMiddle, "crop/layers/middle/temperature[value]");
    InputRef(double, cropTemperatureBottom, "crop/layers/bottom/temperature[value]");
//    InputRef(double, condensationRateCover, "../cover/condensation[vapourFlux]");

    Output(double, heatFluxOutside);
    Output(double, heatFluxInside);
    Output(double, radiationFluxSky);
    Output(double, radiationFluxSunCover);
    Output(double, radiationFluxSunScreens);
    Output(double, radiationFluxCropTop);
    Output(double, radiationFluxCropMiddle);
    Output(double, radiationFluxCropBottom);
    Output(double, coverTemperature);
    Output(double, screensTemperature);
}

void EnergyFluxShelters::reset() {
    EnergyFluxBase::reset();
    heatFluxOutside =
    heatFluxInside =
    radiationFluxSky =
    radiationFluxSunCover =
    radiationFluxSunScreens =
    radiationFluxCropTop =
    radiationFluxCropMiddle =
    radiationFluxCropBottom = 0.;
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
        heatFluxOutside = U*(outdoorsTemperature - coverTemperature)*coverPerGroundArea;;
        heatFluxInside = U*(indoorsTemperature2 - coverTemperature)*coverPerGroundArea;;
        radiationFluxSky = incomingIrAbsorptivity*Sigma*(p4K(skyTemperature) - p4K(coverTemperature))*coverPerGroundArea;


        radiationFluxSunCover = lightAbsorbedCover;
        radiationFluxSunScreens = lightAbsorbedScreens;

        radiationFluxCropTop = radiationFluxCrop(cropTemperatureTop);
        radiationFluxCropMiddle = radiationFluxCrop(cropTemperatureMiddle);
        radiationFluxCropBottom = radiationFluxCrop(cropTemperatureBottom);

        double radiationFluxCropTotal =
            radiationFluxCropTop + radiationFluxCropMiddle + radiationFluxCropBottom;
        double radiationFluxShelter =
                heatFluxOutside +
                heatFluxInside +
                radiationFluxSky +
                radiationFluxSunCover +
                radiationFluxSunScreens +
                radiationFluxCropTotal +
                heatFluxInside;
        double radiationFluxInside =
                heatFluxOutside +
                radiationFluxCropTotal;

        value = -radiationFluxInside;
        coverTemperature += radiationFluxShelter*dt/(heatCapacityCover+heatCapacityScreens);
        indoorsTemperature2 -= radiationFluxInside*dt/Cair;
    }
    value /= n;
}

double EnergyFluxShelters::radiationFluxCrop(double cropTemperature) const {
    return outgoingIrAbsorptivity*Sigma*(p4K(cropTemperature) - p4K(coverTemperature));
//    return outgoingIrAbsorptivity*(
//            Sigma*(p4K(cropTemperature) - p4K(screensTemperature))*maxState +
//            Sigma*(p4K(cropTemperature) - p4K(coverTemperature))*(1-maxState));
}

//        double fluxCoverProp{0.5}, outgoingIrAbsorptivityCover{0};
//        double fluxCoverProp =
//            outgoingIrAbsorptivityCover/(outgoingIrAbsorptivityCover+outgoingIrAbsorptivityScreens);
//        double emCoverScreens =
//            jointEmissivity(outgoingIrAbsorptivityCover, outgoingIrAbsorptivityScreens);
//        double radiationFluxCoverToScreens =
//            emCoverScreens*Sigma*(p4K(coverTemperature) - p4K(screensTemperature));

} //namespace

