/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/utilities.h>
#include "prostephanus.h"

using namespace UniSim;

namespace grainstore{

Prostephanus::Prostephanus(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    setRecursionPolicy(Component::Update, Component::ChildrenNot);
    new Parameter<double>("lossDemandRate", &lossDemandRate, 0.000659, this, "desc");
    new Parameter<double>("lossSearchRate", &lossSearchRate, 0.00795, this, "desc");
    new Parameter<double>("damageDemandRate", &damageDemandRate, 0.00317, this, "desc");
    new Parameter<double>("damageSearchRate", &damageSearchRate, 0.0301, this, "desc");
}

void Prostephanus::initialize()
{
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

