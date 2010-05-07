/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/utilities.h>
#include "prostephanus.h"

namespace grainstore{

Prostephanus::Prostephanus(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    setRecursionPolicy(Component::Update, Component::ChildrenNot);
}

void Prostephanus::initialize()
{
    setParameter("lossDemandRate", &lossDemandRate, 0.000659);
    setParameter("lossSearchRate", &lossSearchRate, 0.00795);
    setParameter("damageDemandRate", &damageDemandRate, 0.00317);
    setParameter("damageSearchRate", &damageSearchRate, 0.0301);

    immigration = seekOneChild<Model*>("immigration");
    development = seekOneChild<Model*>("development");
    reproduction = seekOneChild<Model*>("reproduction");
    emigration = seekOneChild<Model*>("emigration");
}

void Prostephanus::reset()
{
}

void Prostephanus::update()
{
    immigration->deepUpdate();
    development->deepUpdate();
    reproduction->deepUpdate();
    emigration->deepUpdate();
}

} //namespace

