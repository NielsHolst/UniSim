/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/utilities.h>
#include "insect4.h"

using namespace UniSim;


namespace beehive{

Insect4::Insect4(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    setRecursionPolicy(Component::Update, Component::ChildrenNot);
}

void Insect4::initialize() {
    setParameter("initEggs", &initEggs, 30.);

    egg = seekOneChild<Model*>("egg");
    larva = seekOneChild<Model*>("larva");
    pupa = seekOneChild<Model*>("pupa");
    adult = seekOneChild<Model*>("adult");
    fecundity = adult->seekOneChild<Model*>("fecundity");
}

void Insect4::reset() {
    egg->setInput("inflow", initEggs);
}

void Insect4::update() {
    egg->deepUpdate();

    larva->setInput("inflow", egg->state("outflow"));
    larva->deepUpdate();

    pupa->setInput("inflow", larva->state("outflow"));
    pupa->deepUpdate();

    adult->setInput("inflow", pupa->state("outflow"));
    adult->deepUpdate();

    egg->setInput("inflow", fecundity->state("eggsLaid"));
}

} //namespace
