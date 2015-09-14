/* Copyright (C) 2013 by Oliver Koerner, AgroTeh [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "energy_flux_crop.h"
#include "general.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(EnergyFluxCrop)

/*! \class EnergyFluxCrop
 * \brief Flux of thermal heat to greenhouse air
 * Inputs
 * ------
 * - _indoorsTemperature_ is the ambient temperature indoors [<SUP>oC</SUP>C]
 * - _averageHeight_ is the average height of the greenhouse [m]
 * - _timeStep_ is the integration time step [s]
 *
 */

EnergyFluxCrop::EnergyFluxCrop(Identifier name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    InputRef(double, cropTemperature, "crop/temperature[value]");
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, rbH2O, "crop/layers/top/rb[rbH2O]");
    InputRef(double, lai,"crop/lai[value]");
    InputRef(double, fractionPlantArea, "crop/lai[fractionPlantArea]");
    InputRef(double, averageHeight,"geometry[indoorsAverageHeight]");
    InputRef(double, timeStep,"calendar[timeStepSecs]");
}

void EnergyFluxCrop::update() {
    double fluxFromCropToAir =
           CpAir*RhoAir/rbH2O*lai*fractionPlantArea;          // W/m2/K = J/kg/K * kg/m3 * m/s * m2/m2

    double indoorsHeatCapacity =
           averageHeight*RhoAir*CpAir;                        // J/m2/K = m * kg/m3 * J/kg/K

    double rateOfChangeIndoorsTemperature =
           fluxFromCropToAir/indoorsHeatCapacity;             // s-1 = W/m2/K / (J/m2/K)

    double changeIndoorsTemperature =                         // K = K*s/s
           (cropTemperature - indoorsTemperature)*(1. - exp(-rateOfChangeIndoorsTemperature*timeStep));

    value = changeIndoorsTemperature*indoorsHeatCapacity/timeStep;  // W/m2 = K * J/m2/K / s
}

} //namespace

