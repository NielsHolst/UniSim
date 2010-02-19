/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "area.h"
#include "organ.h"

namespace intercom{

Organ::Organ(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    setRecursionPolicy(Component::Update, Component::ChildrenNot);
    setState("absorption", &absorption);
    setState("assimilation", &assimilation);
}

void Organ::initialize()
{
    area = findChild<Area*>("area");
}

void Organ::reset()
{
}

void Organ::update()
{
    area->deepUpdate();
    absorption = area->state("absorption");
    assimilation = area->state("assimilation");
}

LightComponents Organ::weightedAreaAboveLayer(double layerHeight) {
    return area->weightedAreaAboveLayer(layerHeight);
}

} //namespace

