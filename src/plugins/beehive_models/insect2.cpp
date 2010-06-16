/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/utilities.h>
#include "insect2.h"

using namespace UniSim;


namespace beehive{

Insect2::Insect2(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    setRecursionPolicy(Component::Update, Component::ChildrenNot);
}

void Insect2::initialize() {
    setParameter("initEggs", &initEggs, 30., "Initial number of insect eggs");

    egg = seekOneChild<Model*>("egg");
    larva = seekOneChild<Model*>("larva");
    pupa = seekOneChild<Model*>("pupa");
    adult = seekOneChild<Model*>("adult");
}

void Insect2::reset() {
    egg->pushVariable("inflow", initEggs);
}

void Insect2::update() {
    egg->deepUpdate();

    larva->pushVariable("inflow", egg->pullVariable("outflow"));
    larva->deepUpdate();

    pupa->pushVariable("inflow", larva->pullVariable("outflow"));
    pupa->deepUpdate();

    adult->pushVariable("inflow", pupa->pullVariable("outflow"));
    adult->deepUpdate();
}

} //namespace

