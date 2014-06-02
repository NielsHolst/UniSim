/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include "early_growth.h"

using namespace UniSim;

namespace intercom{

EarlyGrowth::EarlyGrowth(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("initialArea", &initialArea, 0.05, this, "Area at time zero (cm @Sup 2 per plant)");
    new Parameter<double>("growthRate", &growthRate, 1.0, this, "Exponential growth rate (m @Sup 2 per m @Sup 2 per photothermal time unit");

    new Variable<double>("area", &area, this, "Current area (cm @Sup 2 per plant)");
}

void EarlyGrowth::initialize() {
    photoThermalTime = seekOneNearest<Model*>("time");
}

void EarlyGrowth::reset() {
    area = initialArea;
}

void EarlyGrowth::update() {
    double pt = photoThermalTime->pullValue<double>("total");
    area = initialArea*exp(growthRate*pt);
}

} //namespace

