/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include "general.h"
#include "publish.h"
#include "indoors_ventilation.h"

using namespace UniSim;

namespace vg {

PUBLISH(IndoorsVentilation)

/*! \class IndoorsVentilation
 * \brief Total air ventilation
 *
 * Inputs
 * ------
 * - _timeStep_ is the integration time step [s]
 * - _averageHeight_ is the average height of the greenhouse [m]
 * - _rateVents_ is the ventilation rate through vents per greenhouse area [m<SUP>3</SUP>]/m<SUP>2</SUP>/h]
 * - _rateInfiltration_ is the ventilation rate through infiltration per greenhouse area [m<SUP>3</SUP>]/m<SUP>2</SUP>/h]
 *
 * Outputs
 * ------
 * - _absolute_ is the volume of air per greenhouse area exchanged through ventilation during one time step [m<SUP>3</SUP>]/m<SUP>2</SUP>]
 * - _relative_ is the relative exchange of air through ventilation during one time step  [m<SUP>3</SUP>]/m<SUP>3</SUP>]
 * - _rate_ is the rate of air exchange through ventilation [m<SUP>3</SUP>]/m<SUP>2</SUP>/h]
 *
 * Default dependencies
 * ------------
 * - a _calendar_ model with a _timeStepSecs_ port [s]
 * - a _construction/geometry_ model with an _averageHeight_ port [m]
 * - a _vents_ child model with a _rate_ port [m<SUP>3</SUP>]/m<SUP>2</SUP>/h]
 * - an _infiltration_ child model with a _rate_ port [m<SUP>3</SUP>]/m<SUP>2</SUP>/h]
 */

IndoorsVentilation::IndoorsVentilation(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, timeStep,"calendar[timeStepSecs]");
    InputRef(double, averageHeight, "construction/geometry[averageHeight]");
    InputRef(double, rateVents,"./vents[rate]");
    InputRef(double, rateInfiltration,"./infiltration[rate]");
    Output(double, absolute);
    Output(double, relative);
    Output(double, rate);
}

void IndoorsVentilation::reset() {
    absolute = relative = rate = 0;
}

void IndoorsVentilation::update() {
    double rate1 = rateVents + rateInfiltration;        // m/h   = m3/m2/h
    double absolute1 = rate1/3600*timeStep;      // m3/m2 = m/h / s/h * s
    absolute = averageHeight*(1-exp(-absolute1/averageHeight));
        // For small values: absolute ~= absolute1
        // For large values: absolute ~= averageHeight
    rate = absolute/timeStep*3600;
    relative = absolute/averageHeight;          // m3/m3 = m3/m2 / m
    Q_ASSERT(relative <= 1.);
}

} //namespace

