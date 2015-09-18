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
    InputRef(double, energyHeatingFlux, "heating/supply[value]");
    InputRef(double, dt, "calendar[timeStepSecs]");
    Output(double, energyHeatingTotal);
}

void Budget::reset() {
    energyHeatingFlux = energyHeatingTotal = 0.;
}

void Budget::update() {
    energyHeatingTotal += energyHeatingFlux*dt/3600./1000.;     // kWh/m2
}


} //namespace

