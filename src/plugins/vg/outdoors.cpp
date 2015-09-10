/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "outdoors.h"
#include "general.h"
#include "publish.h"

using namespace UniSim;

namespace vg {
	
PUBLISH(Outdoors)

/*! \class Outdoors
 * \brief Outdoors weather conditions
 *
 * Inputs (also used as outputs)
 * -----------------------------
 * - _co2_ is the CO<SUB>2</SUB> concentration [ppm]
 * - _temperature_ is ambient temperature [<SUP>o</SUP>C]
 * - _rh_ is the relative humidity of the air [0;100]
 * - _ah_ is the absolute humidity of the air [kg/m<SUP>3</SUP>]
 * - _sh_ is the specific humidity of the air [kg/kg]
 * - _radiation_ is total sunlight irradiation (_diffuseRadiation_+_directRadiation_) [W/m<SUP>2</SUP>]
 * - _diffuseRadiation_ is the diffuse component of sunlight irradiation [W/m<SUP>2</SUP>]
 * - _windSpeed_ is the average wind speed [m/s]
 * - _skyTemperature_ is the temperature of the sky [<SUP>o</SUP>C]
 *
 * Outputs
 * -------
 * - _directRadiation_ is the direct component of sunlight irradiation [W/m<SUP>2</SUP>]
 *
 * Default dependencies
 * ------------
 * - a _records_ child model with the following ports:
 *   + _temperature_ [<SUP>o</SUP>C]
 *   + _rh_ [0;100]
 *   + _radiation_ [W/m<SUP>2</SUP>]
 *   + _diffuseRadiation_ [W/m<SUP>2</SUP>]
 *   + _windSpeed_ [m/s]
 *   + _skyTemperature_ [<SUP>o</SUP>C]
 */

Outdoors::Outdoors(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, co2, 350);
    Input(double, soilTemperature, 10);
    InputRef(double, temperature, "./records[Tair]");
    InputRef(double, rh, "./records[RHair]");
    InputRef(double, radiation, "./records[GlobRad]");
    InputRef(double, diffuseRadiation, "./records[DifRad]");
    InputRef(double, windSpeed, "./records[WindSpeed]");
    InputRef(double, skyTemperature, "./records[Tsky]");
    Output(double, directRadiation);
    Output(double, ah);
    Output(double, sh);
}

void Outdoors::reset() {
    update();
}


void Outdoors::update() {
    directRadiation = radiation - diffuseRadiation;
    ah = ahFromRh(temperature, rh);
    sh = shFromRh(temperature, rh);
}

} //namespace
