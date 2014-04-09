/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include <usbase/test_num.h>
#include "day_degree_scale.h"


using namespace UniSim;

namespace xpest {
	
DayDegreeScale::DayDegreeScale(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(temperature), "weather[Tavg]");
    addParameterRef<double>(Name(cropGrowthStage), "..[growthStage]");
    addParameter<double>(Name(from), 0., "Initial growth stage");
    addParameter<double>(Name(to), 1., "Final growth stage");
    addParameter<double>(Name(T0), 0., "Lower threshold for development (oC)");
    addParameter<double>(Name(duration), 50., "Duration of growth stage (DD)");
    addVariable<double>(Name(growthStage), "Growth stage");
    addVariable<bool>(Name(completed), "Has stage been completed?");
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

