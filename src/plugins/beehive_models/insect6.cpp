/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/utilities.h>
#include "insect6.h"

using namespace UniSim;


namespace beehive{

Insect6::Insect6(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    setRecursionPolicy(Component::Update, Component::ChildrenNot);
}

void Insect6::initialize() {
    setParameter("initEggs", &initEggs, 30.);

    egg = seekOneChild<Model*>("egg");
    larva = seekOneChild<Model*>("larva");
    pupa = seekOneChild<Model*>("pupa");
    adult = seekOneChild<Model*>("adult");
    fecundity = adult->seekOneChild<Model*>("fecundity");
    eggSurvival = egg->seekOneChild<Model*>("survival");
}

void Insect6::reset() {
    egg->pushVariable("inflow", initEggs);
}

void Insect6::update() {
    egg->deepUpdate();

    larva->pushVariable("inflow", egg->pullVariable("outflow"));
    larva->deepUpdate();

    pupa->pushVariable("inflow", larva->pullVariable("outflow"));
    pupa->deepUpdate();

    adult->pushVariable("inflow", pupa->pullVariable("outflow"));
    adult->deepUpdate();

    egg->pushVariable("inflow", fecundity->pullVariable("eggsLaid"));

    // Apply survival as finite growth rate which must be > 0
    double fgr = eggSurvival->pullVariable("survival");
    if (fgr == 0.)
        fgr = 1e-6;
    egg->pushVariable("growthRate", fgr);
}

} //namespace

