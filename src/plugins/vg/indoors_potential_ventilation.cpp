/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include "general.h"
#include "publish.h"
#include "indoors_potential_ventilation.h"

using namespace UniSim;

namespace vg {

PUBLISH(IndoorsPotentialVentilation)

/*! \class IndoorsPotentialVentilation
 * \brief Potential ventilation of greenhouse
 *
 * Inputs
 * ------
 * - _timeStep_ is the integration time step [s]
 * - _rateVents_ is the ventilation rate through vents per greenhouse area [m<SUP>3</SUP>]/m<SUP>2</SUP>/h]
 * - _rateInfiltration_ is the ventilation rate through infiltration per greenhouse area [m<SUP>3</SUP>]/m<SUP>2</SUP>/h]
 * - _screenAirTransmission_ is the relative proportion of air let through cover and screens [0;1]
 *
 * Outputs
 * ------
 * - _rate_ is the potential rate of air exchange through ventilation [m<SUP>3</SUP>]/m<SUP>2</SUP>/h]
 * - _finite_ is is the potential finite rate, during one time step, of air exchange through ventilation [m<SUP>3</SUP>]/m<SUP>2</SUP>]
 *
 * Default dependencies
 * ------------
 * - a _calendar_ model with a _timeStepSecs_ port [s]
 * - a _vents_ child model with a _rate_ port [m<SUP>3</SUP>]/m<SUP>2</SUP>/h]
 * - an _infiltration_ child model with a _rate_ port [m<SUP>3</SUP>]/m<SUP>2</SUP>/h]
 */

IndoorsPotentialVentilation::IndoorsPotentialVentilation(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, timeStep,"calendar[timeStepSecs]");
    InputRef(double, rateVents,"./vents[rate]");
    InputRef(double, rateInfiltration,"./infiltration[rate]");
    InputRef(double, screenAirTransmission, "indoors/screens/transmission[air]");
    Output(double, rate);
    Output(double, finite);
}

void IndoorsPotentialVentilation::reset() {
    finite = rate = 0;
}

void IndoorsPotentialVentilation::update() {
    rate = screenAirTransmission*(rateVents + rateInfiltration);        // m3/m2/h
    finite = rate/3600*timeStep;      // m3/m2 = m3/m2/h / s/h * s
}

} //namespace

