/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include "general.h"
#include "proportional_control.h"

using namespace UniSim;

namespace vg {
	
ProportionalControl::ProportionalControl(Identifier name, QObject *parent)
    : DirectedControl(name, parent)
{
    addParameter<double>(Name(actualValue), 20.,
        "Current value being regulated");
    addParameter<double>(Name(targetValue), 20.,
        "Value to regulate towards");
    addParameter<double>(Name(gapMultiplier), 1.,
        "Multiplier on gap between @F actualValue and @F {targetValue}");
    addParameter<double>(Name(pBand), 5.,
        "Proportional band of response");
    addParameter<double>(Name(maxSignal), 100.,
        "Maximum absolute value of @F {signal}");
    addParameter<bool>(Name(signalNotNegative), true,
        "Should a negative @F signal be set to zero?");

    addVariable<double>(Name(signal),
        "Response between zero and @F maxSignal, increasing with increasing difference between "
        "@F actualValue and @F {targetValue}, positive or negative according to direction of change "
        "and setting of @F signalNotNegative flag");
}

void ProportionalControl::reset() {
    signal = 0;
}

void ProportionalControl::update() {
    double gap = gapMultiplier*(actualValue - targetValue);
    if (direction() == Ceiling)
        gap = -gap;
    signal = propControl(gap, pBand, maxSignal);
    if (signal < 0. && signalNotNegative)
        signal = 0.;
}


} //namespace

