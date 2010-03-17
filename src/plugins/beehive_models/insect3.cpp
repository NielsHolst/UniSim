/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/utilities.h>
#include "insect3.h"

using namespace UniSim;


namespace beehive{

Insect3::Insect3(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    setRecursionPolicy(Component::Update, Component::ChildrenNot);
}

void Insect3::initialize() {
    setParameter("initEggs", &initEggs, 30.);

    egg = UniSim::findChild<Model*>("egg", this);
    larva = UniSim::findChild<Model*>("larva", this);
    pupa = UniSim::findChild<Model*>("pupa", this);
    adult = UniSim::findChild<Model*>("adult", this);
}

void Insect3::reset() {
    egg->setInput("inflow", initEggs);
}

void Insect3::update() {
    egg->deepUpdate();

    larva->setInput("inflow", egg->state("outflow"));
    larva->deepUpdate();

    pupa->setInput("inflow", larva->state("outflow"));
    pupa->deepUpdate();

    adult->setInput("inflow", pupa->state("outflow"));
    adult->deepUpdate();
}

} //namespace

