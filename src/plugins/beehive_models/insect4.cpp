/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/utilities.h>
#include "insect4.h"

using namespace UniSim;


namespace beehive{

Insect4::Insect4(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("initEggs", &initEggs, 30., this, "Initial number of insect eggs");
    setRecursionPolicy(Component::Update, Component::ChildrenNot);
}

void Insect4::initialize() {
    egg = seekOneChild<Model*>("egg");
    larva = seekOneChild<Model*>("larva");
    pupa = seekOneChild<Model*>("pupa");
    adult = seekOneChild<Model*>("adult");
    fecundity = adult->seekOneChild<Model*>("fecundity");
}

void Insect4::reset() {
    egg->pushVariable("inflow", initEggs);
}

void Insect4::update() {
    egg->deepUpdate();

    larva->pushVariable("inflow", egg->pullVariable<double>("outflow"));
    larva->deepUpdate();

    pupa->pushVariable("inflow", larva->pullVariable<double>("outflow"));
    pupa->deepUpdate();

    adult->pushVariable("inflow", pupa->pullVariable<double>("outflow"));
    adult->deepUpdate();

    egg->pushVariable("inflow", fecundity->pullVariable<double>("eggsLaid"));
}

} //namespace

