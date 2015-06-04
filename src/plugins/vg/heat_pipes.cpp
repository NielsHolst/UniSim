/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "heat_pipes.h"
#include "publish.h"
using namespace UniSim;

namespace vg {

PUBLISH(HeatPipes)

/*! \class HeatPipes
 * \brief Temperature and effect of all pipes
 *
 * Outputs
 * ------
 * - _maxEnergyFlux_ maximum energy that can be supplied from the heat pipes [W/m<SUP>2</SUP>]
 *
 */

HeatPipes::HeatPipes(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Output(double, maxEnergyFlux);
}

void HeatPipes::initialize() {
    maxEnergyFluxes.clear();
    auto pipes = seekChildren<Model*>("*");
    for (auto pipe : pipes) {
        maxEnergyFluxes << pipe->peekValuePtr<double>("maxEnergyFlux");
    }
}

void HeatPipes::reset() {
    update();
}

void HeatPipes::update() {
    maxEnergyFlux = 0;
    for (auto flux : maxEnergyFluxes)
        maxEnergyFlux += *flux;
}

} //namespace

