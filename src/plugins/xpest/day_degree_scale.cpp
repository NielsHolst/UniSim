/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/test_num.h>
#include "day_degree_scale.h"
#include "publish.h"

using namespace UniSim;

namespace xpest {
	
PUBLISH(DayDegreeScale)

DayDegreeScale::DayDegreeScale(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, temperature, "weather[Tavg]");
    InputRef(double, cropGrowthStage, "..[growthStage]");
    Input(double, from, 0.);        // Initial growth stage
    Input(double, to, 1.);          // Final growth stage"
    Input(double, T0, 0.);          // Lower threshold for development (oC)
    Input(double, duration, 50.);   // Duration of growth stage (DD)

    Output(double, growthStage);    // Growth stage
    Output(bool, completed);      // Has stage been completed?
}

void DayDegreeScale::reset() {
    age = 0.;
    updateFromAge();
}

void DayDegreeScale::update() {
    if (!completed && cropGrowthStage >= from && cropGrowthStage < to) {
        double dT = temperature - T0;
        if (dT > 0.) {
            age += dT/duration;
            if (age > 1.) age = 1.;
            updateFromAge();
        }
    }
}

void DayDegreeScale::updateFromAge() {
    growthStage = from + age*(to - from);
    completed = age == 1.;
}

} //namespace

