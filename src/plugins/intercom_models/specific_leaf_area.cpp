/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include <usbase/utilities.h>
#include "specific_leaf_area.h"

using namespace UniSim;

namespace intercom{

SpecificLeafArea::SpecificLeafArea(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("initial", &initial, 240., this, "Initial specific leaf area at time zero (cm @Sup {2}\"/\"g)");
    new Parameter<double>("slope", &slope, -0.075, this, "Slope on photothermal time ((cm @Sup {2}\"/\"g\"/\"{@Degree}D))");
    new PullVariable<double>("value", &sla, this, "Specific leaf area (cm @Sup {2}\"/\"g)");
}

void SpecificLeafArea::initialize()
{
    photoThermalTime = seekOneNearest<Model*>("time");
}

void SpecificLeafArea::update()
{
    double pt = photoThermalTime->pullVariable<double>("total");
    sla = initial + slope*pt;
}

} //namespace

