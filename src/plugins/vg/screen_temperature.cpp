/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "screen_temperature.h"
#include "general.h"
#include "publish.h"

using namespace UniSim;

namespace vg {
	
PUBLISH(ScreenTemperature)

/*! \class ScreenTemperature
 * \brief Temperature of greenhouse screen (NOT USED)
 *
 * Inputs
 * ------
 * - _U_ is the heat transfer coefficient of the material [W/m<SUP>2</SUP>/K]
 * - _emissivity_ is the effectiveness in emitting energy as thermal radiation [0;1].
 * - _absorption_ is the proportion of light absorbed [0;1]
 * - _density_ is cover density [kg/m<SUP>3</SUP>]
 * - _heatCapacity_ is cover heat capacity [J/kg/K]
 * - _thickness_ is cover thickness [mm]
 * - _indoorsTemperature_ is the ambient temperature indoors [<SUP>o</SUP>C]
 * - _outdoorsTemperature_ is the ambient temperature outdoors [<SUP>o</SUP>C]
 * - _skyTemperature_ is the temperature of the sky [<SUP>o</SUP>C]
 * - _screenTemperature_ is the average temperature of the screens inside this cover [<SUP>o</SUP>C]
 * - _screenState_ is the max. state of the screens below this cover [0;1], where 0=Open and 1=Drawn
 * - _cropTemperature_ is the temperature of the top layer of the crop [<SUP>o</SUP>C]
 * - _windspeed_ is the outdoors windspeed [m/s]
 *
 * Outputs
 * ------
 * - _value_ is the temperature of the greenhouse cover [<SUP>o</SUP>C]

 * Default dependencies
 * ------------
 * - an _indoors/temperature_ model with a _value_ port
 * - an _outdoors_ model with three ports:
 *   + _temperature_ [<SUP>o</SUP>C]
 *   + _skyTemperature_ [<SUP>o</SUP>C]
 *   + _windspeed_ [m/s] [<SUP>o</SUP>C]
 */
ScreenTemperature::ScreenTemperature(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, U, 7.5);
    Input(double, emissivity, 0.84);
    Input(double, absorption, 0.04);
    Input(double, density, 2600.);
    Input(double, heatCapacity, 840.);
    Input(double, thickness, 4.);
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, cropTemperature, "crop/layers/top/temperature[value]");
    InputRef(double, sunlight, "outdoors[radiation]");
    Input(double, heatFluxCondensation, 0.);
    Output(double, value);
}

void ScreenTemperature::reset() {
    value = indoorsTemperature;
}

void ScreenTemperature::update() {
    value = indoorsTemperature;
//    double heatFluxInside = U*(indoorsTemperature - value),     // W/m2/K * K = W/m2
//           radiationFluxSun = absorption*sunlight,
//           radiationFluxCrop = emissivity*Sigma*(p4(cropTemperature));
//    double totalFlux =
//           heatFluxInside +
//           radiationFluxSun +
//           radiationFluxCrop;
//    double heatCapacityPerArea = density*heatCapacity*thickness/1000; // J/m2/K = kg/m3 * J/kg/K * mm * m/mm
//    value += totalFlux/heatCapacityPerArea;
}

} //namespace

