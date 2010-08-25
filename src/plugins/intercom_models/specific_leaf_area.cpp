/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include <usbase/utilities.h>
#include "plant.h"
#include "specific_leaf_area.h"

using namespace UniSim;

namespace intercom{

SpecificLeafArea::SpecificLeafArea(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("initial", &initial, 0.024, this, "description");
    new Parameter<double>("slope", &slope, -7.5e-6, this, "description");
    new PullVariable<double>("sla", &sla, this, "description");
}

void SpecificLeafArea::initialize()
{
    Plant* plant = seekOneAscendant<Plant*>("*");
    photoThermalTime = plant->seekOneDescendant<Model*>("photoThermalTime");
}

void SpecificLeafArea::update()
{
    double pt = photoThermalTime->pullVariable<double>("total");
    sla = initial + slope*pt;
}

} //namespace

