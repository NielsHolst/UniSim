/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/pull_variable.h>
#include <usbase/utilities.h>
#include "plant.h"
#include "specific_leaf_area.h"

using namespace UniSim;

namespace intercom{

SpecificLeafArea::SpecificLeafArea(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new PullVariable("sla", &sla, this);
}

void SpecificLeafArea::initialize()
{
    setParameter("initial", &initial, 0.024);
    setParameter("slope", &slope, -7.5e-6);
    Plant* plant = seekOneAscendant<Plant*>("*");
    photoThermalTime = plant->seekOneDescendant<Model*>("photoThermalTime");
}

void SpecificLeafArea::update()
{
    double pt = photoThermalTime->pullVariable("total");
    sla = initial + slope*pt;
}

} //namespace

