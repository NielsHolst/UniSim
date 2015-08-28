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
 * \brief Flux of energy carried by air flux
 *
 * Inputs
 * ------
 * - _airFlux_ is the rate of air exchange between compartments [h<SUP>-1</SUP>]
 * - _receiverTemperature_ is the air temperature of the receiving compartment [<SUP>oC</SUP>C]
 * - _outdoorsTemperature_ is the air temperature of the donor compartment [<SUP>oC</SUP>C]
 * - _averageHeight_ is the average height of the receiving compartment [m]
 */

EnergyFluxAir::EnergyFluxAir(Identifier name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    Input(double, airFlux, 0.);
    InputRef(double, receiverTemperature, "indoors/temperature[value]");
    InputRef(double, donorTemperature, "outdoors[temperature]");
    InputRef(double, receiverHeight,"geometry[indoorsAverageHeight]");
    Input(double, donorHeight, 0.);
    InputRef(double, timeStep, "calendar[timeStepSecs]");
}

void EnergyFluxAir::update() {
    const int maxTimeStep = 20;                 // Use time steps no larger than this [s]
    double Creceiver = receiverHeight*RhoAir*CpAir,   // J/m2/K = m * J/kg/K * kg/m3
           Cdonor = donorHeight*RhoAir*CpAir ;
    int n = int(timeStep/maxTimeStep) + 1;
    double dt = timeStep/n;
    value = 0.;
    double receiverTemperature2 = receiverTemperature,
           donorTemperature2 = donorTemperature;
    for (int i=0; i < n; ++i) {
        double dT = donorTemperature2 - receiverTemperature2;
        // J/m2 = J/m2/K * h-1 / (s/h) * K * s
        double Ereceiver = Creceiver*airFlux/3600*dT*dt;
        double dTreceiver = Ereceiver/Creceiver;
        receiverTemperature2 += dTreceiver;

        if (donorHeight > 0.) {
            double Edonor = -Ereceiver;
            double dTdonor = Edonor/Cdonor;
            donorTemperature2 += dTdonor;
        }

        value += Ereceiver;
    }
    value /= timeStep;
}

} //namespace

