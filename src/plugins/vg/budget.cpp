/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "budget.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(Budget)

/*! \class Budget
 * \brief
 *
 * Inputs
 * ------
 *
 * Output
 * ------
 */

Budget::Budget(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, convectionToAir, "heating/supply[value]");
    InputRef(double, radiationToCrop, "crop/heatingAbsorbed[value]");
    InputRef(double, dt, "calendar[timeStepSecs]");
    Output(double, energyHeatingFlux);
    Output(double, energyHeatingTotal);
}

void Budget::reset() {
    energyHeatingFlux = energyHeatingTotal = 0.;
}

void Budget::update() {
    energyHeatingFlux = convectionToAir + radiationToCrop;      // W/m2
    energyHeatingTotal += energyHeatingFlux*dt/3600./1000.;     // kWh/m2
}


} //namespace

